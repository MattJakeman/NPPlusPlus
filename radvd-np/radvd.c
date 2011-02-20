/*
 *   $Id: radvd.c,v 1.34 2008/01/24 10:03:17 psavola Exp $
 *
 *   Authors:
 *    Pedro Roque		<roque@di.fc.ul.pt>
 *    Lars Fenneberg		<lf@elemental.net>	 
 *
 *   This software is Copyright 1996-2000 by the above mentioned author(s), 
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */

#include <config.h>
#include <includes.h>
#include <radvd.h>
#include <pathnames.h>
#include <pthread.h>

struct Interface *IfaceList = NULL;

char usage_str[] =
	"[-hsv] [-d level] [-C config_file] [-m log_method] [-l log_file]\n"
	"\t[-f facility] [-p pid_file] [-u username] [-t chrootdir]";

#ifdef HAVE_GETOPT_LONG
struct option prog_opt[] = {
	{"debug", 1, 0, 'd'},
	{"config", 1, 0, 'C'},
	{"pidfile", 1, 0, 'p'},
	{"logfile", 1, 0, 'l'},
	{"logmethod", 1, 0, 'm'},
	{"facility", 1, 0, 'f'},
	{"username", 1, 0, 'u'},
	{"chrootdir", 1, 0, 't'},
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{"singleprocess", 0, 0, 's'},
	{NULL, 0, 0, 0}
};
#endif

extern FILE *yyin;

char *conf_file = NULL;
char *pname;
int sock = -1;

volatile int sighup_received = 0;
volatile int sigterm_received = 0;
volatile int sigint_received = 0;

void sighup_handler(int sig);
void sigterm_handler(int sig);
void sigint_handler(int sig);
void timer_handler(void *data);
void config_interface(void);
void kickoff_adverts(void);
void stop_adverts(void);
void version(void);
void usage(void);
int drop_root_privileges(const char *);
int readin_config(char *);
int check_conffile_perm(const char *, const char *);

/*
* NP++
*/
static pthread_t icmp6_listener;
int sock_fd;

int
main(int argc, char *argv[])
{
	unsigned char msg[MSG_SIZE];
	char pidstr[16];
	int c, log_method;
	char *logfile, *pidfile;
	sigset_t oset, nset;
	int facility, fd;
	char *username = NULL;
	char *chrootdir = NULL;
	int singleprocess = 0;
#ifdef HAVE_GETOPT_LONG
	int opt_idx;
#endif

	pname = ((pname=strrchr(argv[0],'/')) != NULL)?pname+1:argv[0];

	srand((unsigned int)time(NULL));

	log_method = L_STDERR_SYSLOG;
	logfile = PATH_RADVD_LOG;
	conf_file = PATH_RADVD_CONF;
	facility = LOG_FACILITY;
	pidfile = PATH_RADVD_PID;

	/* parse args */
#ifdef HAVE_GETOPT_LONG
	while ((c = getopt_long(argc, argv, "d:C:l:m:p:t:u:vhs", prog_opt, &opt_idx)) > 0)
#else
	while ((c = getopt(argc, argv, "d:C:l:m:p:t:u:vhs")) > 0)
#endif
	{
		switch (c) {
		case 'C':
			conf_file = optarg;
			break;
		case 'd':
			set_debuglevel(atoi(optarg));
			break;
		case 'f':
			facility = atoi(optarg);
			break;
		case 'l':
			logfile = optarg;
			break;
		case 'p':
			pidfile = optarg;
			break;
		case 'm':
			if (!strcmp(optarg, "syslog"))
			{
				log_method = L_SYSLOG;
			}
			else if (!strcmp(optarg, "stderr_syslog"))
			{
				log_method = L_STDERR_SYSLOG;
			}
			else if (!strcmp(optarg, "stderr"))
			{
				log_method = L_STDERR;
			}
			else if (!strcmp(optarg, "logfile"))
			{
				log_method = L_LOGFILE;
			}
			else if (!strcmp(optarg, "none"))
			{
				log_method = L_NONE;
			}
			else
			{
				fprintf(stderr, "%s: unknown log method: %s\n", pname, optarg);
				exit(1);
			}
			break;
		case 't':
			chrootdir = strdup(optarg);
			break;
		case 'u':
			username = strdup(optarg);
			break;
		case 'v':
			version();
			break;
		case 's':
			singleprocess = 1;
			break;
		case 'h':
			usage();
#ifdef HAVE_GETOPT_LONG
		case ':':
			fprintf(stderr, "%s: option %s: parameter expected\n", pname,
				prog_opt[opt_idx].name);
			exit(1);
#endif
		case '?':
			exit(1);
		}
	}

	if (chrootdir) {
		if (!username) {
			fprintf(stderr, "Chroot as root is not safe, exiting\n");
			exit(1);
		}
		
		if (chroot(chrootdir) == -1) {
			perror("chroot");
			exit (1);
		}
		
		if (chdir("/") == -1) {
			perror("chdir");
			exit (1);
		}
		/* username will be switched later */
	}
	
	if (log_open(log_method, pname, logfile, facility) < 0)
		exit(1);

	flog(LOG_INFO, "version %s started", VERSION);

	/* get a raw socket for sending and receiving ICMPv6 messages */
	sock = open_icmpv6_socket();
	if (sock < 0)
		exit(1);

	/* check that 'other' cannot write the file
         * for non-root, also that self/own group can't either
         */
	if (check_conffile_perm(username, conf_file) < 0) {
		if (get_debuglevel() == 0)
			exit(1);
		else
			flog(LOG_WARNING, "Insecure file permissions, but continuing anyway");
	}
	
	/* if we know how to do it, check whether forwarding is enabled */
	if (check_ip6_forwarding()) {
		if (get_debuglevel() == 0) {
			flog(LOG_ERR, "IPv6 forwarding seems to be disabled, exiting");
			exit(1);
		}
		else
			flog(LOG_WARNING, "IPv6 forwarding seems to be disabled, but continuing anyway.");
	}

	/* parse config file */
	if (readin_config(conf_file) < 0)
		exit(1);

	/* drop root privileges if requested. */
	if (username) {
		if (!singleprocess) {
		 	dlog(LOG_DEBUG, 3, "Initializing privsep");
		 	if (privsep_init() < 0)
				flog(LOG_WARNING, "Failed to initialize privsep.");
		}

		if (drop_root_privileges(username) < 0)
			exit(1);
	}

	/* FIXME: not atomic if pidfile is on an NFS mounted volume */	
	if ((fd = open(pidfile, O_CREAT|O_EXCL|O_WRONLY, 0644)) < 0)
	{
		flog(LOG_ERR, "radvd pid file already exists or cannot be created, terminating: %s", strerror(errno));
		exit(1);
	}
	
	/*
	 * okay, config file is read in, socket and stuff is setup, so
	 * lets fork now...
	 */

	if (get_debuglevel() == 0) {

		/* Detach from controlling terminal */
		if (daemon(0, 0) < 0)
			perror("daemon");

		/* close old logfiles, including stderr */
		log_close();
		
		/* reopen logfiles, but don't log to stderr unless explicitly requested */
		if (log_method == L_STDERR_SYSLOG)
			log_method = L_SYSLOG;
		if (log_open(log_method, pname, logfile, facility) < 0)
			exit(1);

	}

	/*
	 *	config signal handlers, also make sure ALRM isn't blocked and raise a warning if so
	 *      (some stupid scripts/pppd appears to do this...)
	 */
	sigemptyset(&nset);
	sigaddset(&nset, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &nset, &oset);
	if (sigismember(&oset, SIGALRM))
		flog(LOG_WARNING, "SIGALRM has been unblocked. Your startup environment might be wrong.");

	signal(SIGHUP, sighup_handler);
	signal(SIGTERM, sigterm_handler);
	signal(SIGINT, sigint_handler);

	snprintf(pidstr, sizeof(pidstr), "%d\n", getpid());
	
	write(fd, pidstr, strlen(pidstr));
	
	close(fd);

	config_interface();
	kickoff_adverts();

	pthread_t *negt;

//	pthread_create(&negt, NULL, np_listener, (void *)NULL);
	np_listener();
	/* enter loop */

	for (;;)
	{
		int len, hoplimit;
		struct sockaddr_in6 rcv_addr;
		struct in6_pktinfo *pkt_info = NULL;
		
		len = recv_rs_ra(sock, msg, &rcv_addr, &pkt_info, &hoplimit);
		if (len > 0)
			process(sock, IfaceList, msg, len, 
				&rcv_addr, pkt_info, hoplimit);

		if (sigterm_received || sigint_received) {
			stop_adverts();
			break;
		}

		if (sighup_received)
		{
			reload_config();		
			sighup_received = 0;
		}
	}
	
	unlink(pidfile);
	exit(0);
}

void
timer_handler(void *data)
{
	struct Interface *iface = (struct Interface *) data;
	double next;

	dlog(LOG_DEBUG, 4, "timer_handler called for %s", iface->Name);

	send_ra(sock, iface, NULL);

	next = rand_between(iface->MinRtrAdvInterval, iface->MaxRtrAdvInterval); 

	if (iface->init_racount < MAX_INITIAL_RTR_ADVERTISEMENTS)
	{
		iface->init_racount++;
		next = min(MAX_INITIAL_RTR_ADVERT_INTERVAL, next);
	}

	set_timer(&iface->tm, next);
}

void
config_interface(void)
{
	struct Interface *iface;
	for(iface=IfaceList; iface; iface=iface->next)
	{
		if (iface->AdvLinkMTU)
			set_interface_linkmtu(iface->Name, iface->AdvLinkMTU);
		if (iface->AdvCurHopLimit)
			set_interface_curhlim(iface->Name, iface->AdvCurHopLimit);
		if (iface->AdvReachableTime)
			set_interface_reachtime(iface->Name, iface->AdvReachableTime);
		if (iface->AdvRetransTimer)
			set_interface_retranstimer(iface->Name, iface->AdvRetransTimer);
	}
}

void
kickoff_adverts(void)
{
	struct Interface *iface;

	/*
	 *	send initial advertisement and set timers
	 */

	for(iface=IfaceList; iface; iface=iface->next)
	{
		if( ! iface->UnicastOnly )
		{
			init_timer(&iface->tm, timer_handler, (void *) iface);
			if (iface->AdvSendAdvert)
			{
				/* send an initial advertisement */
				send_ra(sock, iface, NULL);

				iface->init_racount++;

				set_timer(&iface->tm,
					  min(MAX_INITIAL_RTR_ADVERT_INTERVAL,
					      iface->MaxRtrAdvInterval));
			}
		}
	}
}

void
stop_adverts(void)
{
	struct Interface *iface;

	/*
	 *	send final RA (a SHOULD in RFC2461 section 6.2.5)
	 */

	for (iface=IfaceList; iface; iface=iface->next) {
		if( ! iface->UnicastOnly ) {
			if (iface->AdvSendAdvert) {
				/* send a final advertisement with zero Router Lifetime */
				iface->AdvDefaultLifetime = 0;
				send_ra(sock, iface, NULL);
			}
		}
	}
}

void reload_config(void)
{
	struct Interface *iface;

	flog(LOG_INFO, "attempting to reread config file");

	dlog(LOG_DEBUG, 4, "reopening log");
	if (log_reopen() < 0)
		exit(1);

	/* disable timers, free interface and prefix structures */
	for(iface=IfaceList; iface; iface=iface->next)
	{
		/* check that iface->tm was set in the first place */
		if (iface->tm.next && iface->tm.prev)
		{
			dlog(LOG_DEBUG, 4, "disabling timer for %s", iface->Name);
			clear_timer(&iface->tm);
		}
	}

	iface=IfaceList; 
	while(iface)
	{
		struct Interface *next_iface = iface->next;
		struct AdvPrefix *prefix;
		struct AdvRoute *route;
		struct AdvRDNSS *rdnss;
		/*
		* NP++
		*/
		struct np_map_conf *npmc;

		dlog(LOG_DEBUG, 4, "freeing interface %s", iface->Name);
		
		prefix = iface->AdvPrefixList;
		while (prefix)
		{
			struct AdvPrefix *next_prefix = prefix->next;
			
			free(prefix);
			prefix = next_prefix;
		}
		
		route = iface->AdvRouteList;
		while (route)
		{
			struct AdvRoute *next_route = route->next;

			free(route);
			route = next_route;
		}
		
		npmc = iface->NpMapList;
	    while(npmc)
	    {
	        struct np_map_conf *npmc_next = npmc->next;

			free(npmc);	
	        npmc=npmc->next ;
	    }

		rdnss = iface->AdvRDNSSList;
		while (rdnss) 
		{
			struct AdvRDNSS *next_rdnss = rdnss->next;
			
			free(rdnss);
			rdnss = next_rdnss;
		}	 

		free(iface);
		iface = next_iface;
	}

	IfaceList = NULL;

	/* reread config file */
	if (readin_config(conf_file) < 0)
		exit(1);

	/* XXX: fails due to lack of permissions with non-root user */
	config_interface();
	kickoff_adverts();

	flog(LOG_INFO, "resuming normal operation");
}

void
sighup_handler(int sig)
{
	/* Linux has "one-shot" signals, reinstall the signal handler */
	signal(SIGHUP, sighup_handler);

	dlog(LOG_DEBUG, 4, "sighup_handler called");

	sighup_received = 1;
}

void
sigterm_handler(int sig)
{
	/* Linux has "one-shot" signals, reinstall the signal handler */
	signal(SIGTERM, sigterm_handler);

	dlog(LOG_DEBUG, 4, "sigterm_handler called");

	sigterm_received = 1;
}

void
sigint_handler(int sig)
{
	/* Linux has "one-shot" signals, reinstall the signal handler */
	signal(SIGINT, sigint_handler);

	dlog(LOG_DEBUG, 4, "sigint_handler called");

	sigint_received = 1;
}

int
drop_root_privileges(const char *username)
{
	struct passwd *pw = NULL;
	pw = getpwnam(username);
	if (pw) {
		if (initgroups(username, pw->pw_gid) != 0 || setgid(pw->pw_gid) != 0 || setuid(pw->pw_uid) != 0) {
			flog(LOG_ERR, "Couldn't change to '%.32s' uid=%d gid=%d\n", 
					username, pw->pw_uid, pw->pw_gid);
			return (-1);
		}
	}
	else {
		flog(LOG_ERR, "Couldn't find user '%.32s'\n", username);
		return (-1);
	}
	return 0;
}

int
check_conffile_perm(const char *username, const char *conf_file)
{
	struct stat *st = NULL;
	struct passwd *pw = NULL;
	FILE *fp = fopen(conf_file, "r");

	if (fp == NULL) {
		flog(LOG_ERR, "can't open %s: %s", conf_file, strerror(errno));
		return (-1);
	}
	fclose(fp);

	st = malloc(sizeof(struct stat));
	if (st == NULL)
		goto errorout;

	if (!username)
		username = "root";
	
	pw = getpwnam(username);

	if (stat(conf_file, st) || pw == NULL)
		goto errorout;

	if (st->st_mode & S_IWOTH) {
                flog(LOG_ERR, "Insecure file permissions (writable by others): %s", conf_file);
		goto errorout;
        }

	/* for non-root: must not be writable by self/own group */
	if (strncmp(username, "root", 5) != 0 &&
	    ((st->st_mode & S_IWGRP && pw->pw_gid == st->st_gid) ||
	     (st->st_mode & S_IWUSR && pw->pw_uid == st->st_uid))) {
                flog(LOG_ERR, "Insecure file permissions (writable by self/group): %s", conf_file);
		goto errorout;
        }

	free(st);
        return 0;

errorout:
	if (st)
		free(st);
	return(-1);
}

int
check_ip6_forwarding(void)
{
	int forw_sysctl[] = { SYSCTL_IP6_FORWARDING };
	int value;
	size_t size = sizeof(value);
	FILE *fp = NULL;

#ifdef __linux__
	fp = fopen(PROC_SYS_IP6_FORWARDING, "r");
	if (fp) {
		fscanf(fp, "%d", &value);
		fclose(fp);
	}
	else
		flog(LOG_DEBUG, "Correct IPv6 forwarding procfs entry not found, "
	                       "perhaps the procfs is disabled, "
	                        "or the kernel interface has changed?");
#endif /* __linux__ */

	if (!fp && sysctl(forw_sysctl, sizeof(forw_sysctl)/sizeof(forw_sysctl[0]),
	    &value, &size, NULL, 0) < 0) {
		flog(LOG_DEBUG, "Correct IPv6 forwarding sysctl branch not found, "
			"perhaps the kernel interface has changed?");
		return(0);	/* this is of advisory value only */
	}
	
	if (value != 1) {
		flog(LOG_DEBUG, "IPv6 forwarding setting is: %u, should be 1", value);
		return(-1);
	}
		
	return(0);
}

int
readin_config(char *fname)
{
	if ((yyin = fopen(fname, "r")) == NULL)
	{
		flog(LOG_ERR, "can't open %s: %s", fname, strerror(errno));
		return (-1);
	}

	if (yyparse() != 0)
	{
		flog(LOG_ERR, "error parsing or activating the config file: %s", fname);
		return (-1);
	}
	
	fclose(yyin);
	return 0;
}

void
version(void)
{
	fprintf(stderr, "Version: %s\n\n", VERSION);
	fprintf(stderr, "Compiled in settings:\n");
	fprintf(stderr, "  default config file		\"%s\"\n", PATH_RADVD_CONF);
	fprintf(stderr, "  default pidfile		\"%s\"\n", PATH_RADVD_PID);
	fprintf(stderr, "  default logfile		\"%s\"\n", PATH_RADVD_LOG);
	fprintf(stderr, "  default syslog facililty	%d\n", LOG_FACILITY);
	fprintf(stderr, "Please send bug reports or suggestions to %s.\n",
		CONTACT_EMAIL);

	exit(1);	
}

void
usage(void)
{
	fprintf(stderr, "usage: %s %s\n", pname, usage_str);
	exit(1);	
}

void *np_listener(void)
{

	struct icmp6_filter filter;
    int val;
//    pthread_mutexattr_t mattrs;

    sock_fd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);

    if (sock_fd < 0)
    {
        printf("Unable to open ICMPv6 socket! "
               "Do you have root permissions?\n");
    }

    val = 1;

    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &val, sizeof(val)) < 0)
    {
        printf("Setsockopt() failed in client.c -- 1\n") ;
        return -1;
    }
    if (setsockopt(sock_fd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &val, sizeof(val)) < 0)
    {
        printf("Setsockopt() failed in client.c -- 2\n") ;
        return -1;
    }

    ICMP6_FILTER_SETBLOCKALL(&filter);
    ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &filter);

    ICMP6_FILTER_SETPASS(ND_ROUTER_ADVERT, &filter);
    ICMP6_FILTER_SETPASS(ND_NEIGHBOR_ADVERT, &filter);

    if (setsockopt(sock_fd, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(struct icmp6_filter)) < 0)
    {
        printf("Setsockopt() failed in client.c -- 3\n") ;
        return -1;
    }

    val = 2 ;


    /* create ICMP listener thread */
	if(pthread_create(&icmp6_listener, NULL, icmp6_listen, &sock_fd))
    {
        return -1;
    }
	return 0;
}

void *icmp6_listen(void *arg)
{
    uint8_t msg[MAX_PKT_LEN];
    struct sockaddr_in6 addr;
    struct in6_addr *saddr, *daddr;
    struct in6_pktinfo pkt_info;  
    struct icmp6_hdr *ih;
    int iif, hoplimit;
    ssize_t len;
    int *fd_arg = (int *)arg;
    int sock_fd = *fd_arg;

    while (1)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        len = icmp6_recv(sock_fd, msg, sizeof(msg),
                 &addr, &pkt_info, &hoplimit);
        /* check if socket has closed */
        if (len == -EBADF)
            break;
        /* common validity check */
        if (len < sizeof(struct icmp6_hdr))
            continue;
        saddr = &addr.sin6_addr;
        daddr = &pkt_info.ipi6_addr;
        iif = pkt_info.ipi6_ifindex;

        ih = (struct icmp6_hdr *)msg;
		printf("Message Received In icmp6_listen()\n");
		/*
		* XXX : Call recv function here
		*/
		na_recv(ih, len, saddr, daddr, iif, hoplimit, sock_fd);
    }
    pthread_exit(NULL);
}

ssize_t icmp6_recv(int sockfd, unsigned char *msg, size_t msglen,
           struct sockaddr_in6 *addr, struct in6_pktinfo *pkt_info,
           int *hoplimit)
{
    struct msghdr mhdr;
    struct cmsghdr *cmsg;
    struct iovec iov;
    static unsigned char chdr[CMSG_BUF_LEN];
    ssize_t len;

    iov.iov_len = msglen;
    iov.iov_base = (unsigned char *) msg;

    mhdr.msg_name = (void *)addr;
    mhdr.msg_namelen = sizeof(struct sockaddr_in6);
    mhdr.msg_iov = &iov;
    mhdr.msg_iovlen = 1;
    mhdr.msg_control = (void *)chdr;
    mhdr.msg_controllen = CMSG_BUF_LEN;

    if ((len = recvmsg(sockfd, &mhdr, 0)) < 0)
        return -1;

        for (cmsg = CMSG_FIRSTHDR(&mhdr); cmsg;
         cmsg = CMSG_NXTHDR(&mhdr, cmsg)) {
        if (cmsg->cmsg_level != IPPROTO_IPV6)
            continue;
        switch(cmsg->cmsg_type) {
        case IPV6_HOPLIMIT:
            *hoplimit = *(int *)CMSG_DATA(cmsg);
            break;
        case IPV6_PKTINFO:
            memcpy(pkt_info, CMSG_DATA(cmsg), sizeof(*pkt_info));
            break;
        }
    }
    return len;
}

void na_recv(const struct icmp6_hdr *ih, ssize_t len, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif, int hoplimit, int sock_fd)
{
	if(ih->icmp6_type == ND_NEIGHBOR_ADVERT)
	{
		struct nd_neighbor_advert *na = (struct nd_neighbor_advert *)ih;
		int optlen = len - sizeof(struct nd_neighbor_advert);
		uint8_t *opt = (uint8_t *)(na + 1);
		struct Interface *iface;
		uint8_t mapping = 0;
		char *proc_cat_s = malloc(MAX_PROC_STR_SIZE + 1);
		uint8_t np_fin_recvd = 0;
		uint8_t source_ll_addr_recvd = 0;
		unsigned char *mac;
		char hw_addr[MAX_HWADDR_STR_LEN];
		uint8_t mac_count;
		char temp_str[4];
		
		printf("NA Received\n");
		
		while (optlen > 1)
		{
			int olen = opt[1] << 3;
			if (olen > optlen || olen == 0)
				return;
			switch (opt[0])
			{
				
			  case 1: // Source Link Layer Address
				{
					source_ll_addr_recvd = 1;
					mac = opt + 2;
					printf("MAC From Source Link Layer Option == %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
								 (unsigned char)mac[0],
								 (unsigned char)mac[1],
								 (unsigned char)mac[2],
								 (unsigned char)mac[3],
								 (unsigned char)mac[4],
								 (unsigned char)mac[5]);
					break;
				}
			  case 63:
				{
					mapping = opt[2];
					printf("New Mapping == %d\n", mapping);
					np_fin_recvd = 1;
					break;
				}
			}
			optlen -= olen;
			opt += olen;
		}
		/*
		 * XXX : Get Source LL Addr Here
		 */
		//         for(iface=IfaceList; iface; iface=iface->next)
		//         {
		/*
		 * Addresses match so set the mapping in proc on this address
		 */
		//          if(memcmp(&iface->if_addr, &daddr, sizeof(struct in6_addr)) == 0)
		//            if(iif == iface->if_index)
		//            {

		if(np_fin_recvd == 1)
		{
			memset(&hw_addr, 0, MAX_HWADDR_STR_LEN);
			
			for(mac_count = 0; mac_count < 6; mac_count++)
			{
				if(mac_count < (5))
				{
					snprintf(temp_str, 4, "%.2x:", mac[mac_count]);
					strncat(hw_addr, temp_str, strlen(temp_str));
				}
				else
				{
					snprintf(temp_str, 3, "%.2x", mac[mac_count]);
					strncat(hw_addr, temp_str, strlen(temp_str));
				}
			}
		}

		printf("Source MAC Converted To String == %s\n", hw_addr);

#ifdef __linux__
		if(np_fin_recvd == 1)
		{
			snprintf(proc_cat_s, MAX_PROC_STR_SIZE + 1, "echo %d > /proc/net/np++/maccurmappings/%s", mapping, hw_addr);
			system(proc_cat_s);
		}
#endif
		//            }
		//        }
	}
}

#if 0
#define MYPORT 5555
#define MAXBUFLEN 100
#define BACKLOG 10

void *np_listener(void *args)
{
	int sockfd, new_fd;
    struct sockaddr_in6 my_addr;
    struct sockaddr_in6 their_addr;
    socklen_t addr_len;
    int numbytes;
    char buf[MAXBUFLEN];
	pthread_t *acct;

	printf("Starting negotiation thread\n");

    if ((sockfd = socket(PF_INET6, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset((char *)&my_addr, 0, sizeof(my_addr)) ;

    my_addr.sin6_family = AF_INET6;
    my_addr.sin6_port = htons(MYPORT);
    my_addr.sin6_addr = in6addr_any;

    if (bind(sockfd, (struct sockaddr *)&my_addr,
        sizeof(my_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

	if(listen(sockfd, BACKLOG)==-1)
	{
		perror("listen");
        exit(1);
	}

	while(1)
	{
		printf("Waiting.....\n");
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_len);
		if(new_fd == -1)
		{
			perror("accept");
        	exit(1);
		}
		pthread_create(&acct, NULL, np_accept, (void *)&new_fd);

		printf("Accepted incoming connection\n");
	}

    close(sockfd);

    return 0;
}


void *np_accept(void *args)
{
    int numbytes;
	int *sockfd = (int *) args;
//	struct np_finalise *npf = malloc(sizeof(struct np_finalise));
	struct np_finalise npf;
	struct sockaddr_in6 sin6;
	socklen_t addr_len = sizeof(struct sockaddr_in6);

//	while(1)
	{
//		if((numbytes = recv(*sockfd, (struct np_finalise *)&npf, sizeof(struct np_finalise), 0)) > 0)
		if((numbytes = recv(*sockfd, (struct np_finalise *)&npf, sizeof(struct np_finalise), 0)) > 0)
		{
			char *saddr = malloc(INET6_ADDRSTRLEN);
			char *proc_cat_s = malloc(MAX_PROC_STR_SIZE + 1);

			inet_ntop(AF_INET6, &npf.source, saddr, INET6_ADDRSTRLEN);

    		printf("Finalisation Message received with the following data :\nSource Address : %s\nMapping : %d\n", saddr, npf.mapping);

			if(getsockname(*sockfd, (struct sockaddr *)&sin6, &addr_len) != 0)
			{
				perror("getsockname() : ");
			}

			/*
			* List through interfaces to find one with the address that matches the address of the bound socket the packet was received on
			*/
			struct Interface *iface;
		    for(iface=IfaceList; iface; iface=iface->next)
		    {
//				char *tempin6a[INET6_ADDRSTRLEN];
//				inet_ntop(AF_INET6, &iface->if_addr, tempin6a, INET6_ADDRSTRLEN);
//				char *tempin6a2[INET6_ADDRSTRLEN];
//				inet_ntop(AF_INET6, &sin6.sin6_addr, tempin6a2, INET6_ADDRSTRLEN);
				/*
				* Addresses match so set the mapping in proc on this address
				*/
				if(memcmp(&iface->if_addr, &sin6.sin6_addr, sizeof(struct in6_addr)) == 0)
				{
#ifdef __linux__
					snprintf(proc_cat_s, MAX_PROC_STR_SIZE + 1, "echo %d > /proc/net/np++/ifcurmappings/%s", npf.mapping, iface->Name);
					system(proc_cat_s);
#else
/*
* XXX Write code for BSD to set mapping
*/
#endif
//					printf("Interface Address From List== %s\n", tempin6a);
//					printf("Interface Address From getsockname()== %s\n", tempin6a2);
				}
		    }

			free(saddr);
			close(sockfd);
			free(proc_cat_s);
			pthread_exit(0);
		}
		else if(numbytes == 0)
		{
			return 1;
		}
		else
		{
			perror("recv");
			exit(0);
		}
	}
}

#endif
