#include "mapad.h"
//#include <errno.h>
//#include <string.h>
//#include </usr/src/sys/sys/socket.h>
//#include <netdb.h>
//#include <linux/netdevice.h>

pthread_mutex_t send_mutex;
//static pthread_rwlock_t handler_lock;
static pthread_t icmp6_listener;

struct np_map *my_npm;
uint8_t no_my_maps;
int sock_fd;

int start_client()
{
	struct icmp6_filter filter;
	int val;
//	pthread_mutexattr_t mattrs;
//	pthread_t chart;

	init_my_npm();
//	pthread_create(&chart, NULL, input_thread, (void *)NULL);

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
//    ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &filter);

	ICMP6_FILTER_SETPASS(ND_ROUTER_ADVERT, &filter);
	ICMP6_FILTER_SETPASS(ND_NEIGHBOR_SOLICIT, &filter);
//	ICMP6_FILTER_SETPASS(ND_NEIGHBOR_ADVERT, &filter);


	if (setsockopt(sock_fd, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(struct icmp6_filter)) < 0)
	{
		printf("Setsockopt() failed in client.c -- 3\n") ;
        return -1;
	}

	val = 2 ;

	/* create ICMP listener thread */
//    pthread_mutexattr_init(&mattrs);
//    pthread_mutexattr_settype(&mattrs, PTHREAD_MUTEX_NORMAL);
/*    if (pthread_mutex_init(&send_mutex, &mattrs) ||
        pthread_rwlock_init(&handler_lock, NULL) ||
        pthread_create(&icmp6_listener, NULL, icmp6_listen, &sock_fd))*/
	if(pthread_create(&icmp6_listener, NULL, icmp6_listen, &sock_fd))
	{
        return -1;
	}

    return 0;
}

#if 0
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
//    struct icmp6_handler *h;

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
        /* multiplex to right handler */
//        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
//        pthread_rwlock_rdlock(&handler_lock);
/*        if ((h = icmp6_handler_get(ih->icmp6_type)) != NULL)*/
		if(conf.mode == MAPD_MODE_CLIENT)
	        mapd_client_recv(ih, len, saddr, daddr, iif, hoplimit, sock_fd);
		else if(conf.mode == MAPD_MODE_SERVER)
			mapd_server_recv(ih, len, saddr, daddr, iif, hoplimit, sock_fd);
//        pthread_rwlock_unlock(&handler_lock);
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
#endif

void mapd_client_recv(const struct icmp6_hdr *ih, ssize_t len,
               const struct in6_addr *saddr,
               const struct in6_addr *daddr, int iif, int hoplimit, int sock_fd)
{
//    struct md_inet6_iface *iface;

	if(ih->icmp6_type == ND_ROUTER_ADVERT)
	{
		struct nd_router_advert *ra = (struct nd_router_advert *)ih;
	    int optlen = len - sizeof(struct nd_router_advert);
	    uint8_t *opt = (uint8_t *)(ra + 1);

//	    printf("Message Is A RA\n");
		while (optlen > 1)
		{
	        int olen = opt[1] << 3;

	        if (olen > optlen || olen == 0)
	            return;

			switch (opt[0])
			{
				case 62:
				{
	//				uint8_t type = opt[0];
	//				uint8_t len = opt[1];
					uint8_t no_maps = opt[2];
					int i = 0;
					struct np_map *npm = malloc(no_maps * sizeof(struct np_map));

					printf("RA Received With %d Mappings\n", no_maps);

					int off = 3;

					opt += off;
					for(i = 0; i < no_maps; i++)
					{
						memcpy(&npm[i], opt, sizeof(struct np_map));
	//					printf("Mapping = %d\nprio = %d\n", npm[i].mapping, npm[i].prio);
						off+=sizeof(struct np_map);
						opt+=sizeof(struct np_map);
					}

					negotiate(npm, no_maps, saddr, daddr, iif, sock_fd);
					free(npm);
					break;
				}
			}
			optlen -= olen;
	        opt += olen;
		}



	    return;
	}
	else if(ih->icmp6_type == ND_NEIGHBOR_SOLICIT)
	{
	    struct nd_neighbor_solicit *ns = (struct nd_neighbor_solicit *)ih;
	    int optlen = len - sizeof(struct nd_neighbor_solicit);
	    uint8_t *opt = (uint8_t *)(ns + 1);

//		printf("Message Is A NS\n");

		while (optlen > 1)
		{
	        int olen = opt[1] << 3;

	        if (olen > optlen || olen == 0)
	            return;

			switch (opt[0])
			{
				case 62:
				{
	//				uint8_t type = opt[0];
	//				uint8_t len = opt[1];
					uint8_t no_maps = opt[2];
					int i = 0;
					struct np_map *npm = malloc(no_maps * sizeof(struct np_map));

					printf("NS Received With %d Mappings\n", no_maps);

		            int off = 3;

					opt += off;
					for(i = 0; i < no_maps; i++)
					{
						memcpy(&npm[i], opt, sizeof(struct np_map));
	//					printf("Mapping = %d\nprio = %d\n", npm[i].mapping, npm[i].prio);
						off+=sizeof(struct np_map);
						opt+=sizeof(struct np_map);
					}

					negotiate(npm, no_maps, saddr, daddr, iif, sock_fd);
					free(npm);
					break;
				}
			}
			optlen -= olen;
	        opt += olen;
		}
	}
}


void init_my_npm()
{
/*	no_my_maps = 3;
	my_npm = malloc(3 * sizeof(struct np_map));

	my_npm[0].mapping = 0;
	my_npm[0].prio = 0;

	my_npm[1].mapping = 2;
	my_npm[1].prio = 20;

	my_npm[2].mapping = 3;
	my_npm[2].prio = 1;
*/
//	reload_config();
}

#define SERVER_PORT "5555"
//#define NEG_INTERFACE "eth1"

uint8_t negotiate(struct np_map *npm, uint8_t no_maps, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif, int sock_fd)
{
//    uint8_t all_hosts_addr[] = {0xff,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    struct sockaddr_in6 addr;
    struct nd_neighbor_advert *na;
    struct iovec iov;
    struct cmsghdr *cmsg;
    char chdr[CMSG_SPACE(sizeof(struct in6_pktinfo))];
    struct in6_pktinfo *pkt_info;
    struct msghdr mhdr;
    int i,j = 0;
	float cur_best_prio = -1;
//	struct np_finalise npf;
	struct np_fin_nd_opt npf;
	char *proc_cat_s = malloc(MAX_PROC_STR_SIZE + 1);

    unsigned char buff[MSG_SIZE];
    int len = 0;
    int err;
	struct ifreq ifr;

	uint8_t *ucp;
	unsigned int slla_len;

    memset((void *)&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(IPPROTO_ICMPV6);
    memcpy(&addr.sin6_addr, saddr, sizeof(struct in6_addr));

    memset(&buff, 0, sizeof(buff));

    na = (struct nd_neighbor_advert *) buff;

    na->nd_na_type = ND_NEIGHBOR_ADVERT;
    na->nd_na_code  = 0;
    na->nd_na_cksum = 0;
    na->nd_na_target = *daddr;

    len = sizeof(struct nd_neighbor_advert);

	/*
	* Add the Source LL Address ICMPv6 Option
	*/

	ifr.ifr_addr.sa_family = AF_INET6;
	strncpy(ifr.ifr_name, conf.interface, IF_NAMESIZE-1);

	ioctl(sock_fd, SIOCGIFHWADDR, &ifr);

	printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
			(unsigned char)ifr.ifr_hwaddr.sa_data[0],
			(unsigned char)ifr.ifr_hwaddr.sa_data[1],
			(unsigned char)ifr.ifr_hwaddr.sa_data[2],
			(unsigned char)ifr.ifr_hwaddr.sa_data[3],
			(unsigned char)ifr.ifr_hwaddr.sa_data[4],
			(unsigned char)ifr.ifr_hwaddr.sa_data[5]);

	ucp = (uint8_t *) (buff + len);

	*ucp++  = ND_OPT_SOURCE_LINKADDR;
	*ucp++  = 1; /* XXX : HACK : This will only work for MAC */

	len += 2 * sizeof(uint8_t);

	slla_len = 6; /* XXX : HACK : This will only work for MAC */
	memcpy(buff + len, &ifr.ifr_hwaddr.sa_data, slla_len);
	len+= slla_len;

	/*
	* Add NP++ Finalisation Option
	*/
	npf.type = 63;
	npf.length = 1;

	for(i = 0; i < NP_FIN_PAD_SIZE; i++)
	{
		npf.pad[i] = 0;
	}

	/*
	* Perform comparison of mapping tables to determine best mapping to use
	*/
	for(i = 0; i < no_maps; i++)
	{
		for(j = 0; j < no_my_maps; j++)
		{
			// Check mapping exists in both lists
			if(my_npm[j].mapping == npm[i].mapping)
			{
				float av_prio = 0;
				av_prio = (float)(((float)my_npm[j].prio + (float)npm[i].prio) / 2);
				if(av_prio > cur_best_prio)
				{
					cur_best_prio = av_prio;
					npf.mapping = my_npm[j].mapping;
				}
			}
		}
	}
	printf("New Mapping ID == %d\n", npf.mapping);

	/*
	* NP++ : Copy the ICMPv6 option type, length and no_maps field into the send buffer
	*/
    memcpy(buff + len, &npf, sizeof(npf));
    len += (sizeof(npf));


	iov.iov_len  = len;
    iov.iov_base = (caddr_t) buff;

    memset(chdr, 0, sizeof(chdr));
    cmsg = (struct cmsghdr *) chdr;

    cmsg->cmsg_len   = CMSG_LEN(sizeof(struct in6_pktinfo));
    cmsg->cmsg_level = IPPROTO_IPV6;
    cmsg->cmsg_type  = IPV6_PKTINFO;

    pkt_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
    pkt_info->ipi6_ifindex = conf.ifindex;
    memcpy(&pkt_info->ipi6_addr, &conf.lladdr, sizeof(struct in6_addr));

    addr.sin6_scope_id = conf.ifindex;

    memset(&mhdr, 0, sizeof(mhdr));
    mhdr.msg_name = (caddr_t)&addr;
    mhdr.msg_namelen = sizeof(struct sockaddr_in6);
    mhdr.msg_iov = &iov;
    mhdr.msg_iovlen = 1;
    mhdr.msg_control = (void *) cmsg;
    mhdr.msg_controllen = sizeof(chdr);

    err = sendmsg(sock_fd, &mhdr, 0);

//	printf("NA Received Finalising The Mapping %d - Setting the /proc/ entry\n", mapping);
#ifdef __linux__
	snprintf(proc_cat_s, MAX_PROC_STR_SIZE + 1, "echo %d > /proc/net/np++/ifcurmappings/%s", npf.mapping, conf.interface);
	system(proc_cat_s);
#endif
	free(proc_cat_s);

    if (err < 0)
    {
        perror("sendmsg() : ");
    }



	return 0;
}
#if 0
void *input_thread(void *args)
{
	char c;

	while((c = getchar()))
	{
	  switch(c)
	  {
	    case 'r':
		{
		  reload_config();
		}
	  }
	}
	return NULL;
}
#endif
#if 0
uint8_t negotiate(struct np_map *npm, uint8_t no_maps, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif)
{
    int sockfd;
    int numbytes;
//    struct in6_addr i6a ;
	char *saddr_s = malloc(INET6_ADDRSTRLEN+5);
	int i, j;
	float cur_best_prio = -1;
	char *to_send = malloc(8);
	struct np_finalise npf;
//	struct net_device netdev;

	inet_ntop(AF_INET6, saddr, saddr_s, INET6_ADDRSTRLEN);
	printf("saddr_s == %s\n", saddr_s);
	struct addrinfo *ai, req;

	memset(&req, 0, sizeof(struct addrinfo));
	req.ai_socktype = SOCK_STREAM;

	strcat(saddr_s, "%");
	strcat(saddr_s, conf.interface);

	/*
	* Compare the mappings received to our own and make decision
	*/
	for(i = 0; i < no_maps; i++)
	{
		for(j = 0; j < no_my_maps; j++)
		{
			// Check mapping exists in both lists
			if(my_npm[j].mapping == npm[i].mapping)
			{
				float av_prio = 0;
				av_prio = (float)(((float)my_npm[j].prio + (float)npm[i].prio) / 2);
//				printf("Average priority of mapping %d is %f\n", my_npm[j].mapping, av_prio);
				if(av_prio > cur_best_prio)
				{
					cur_best_prio = av_prio;
					npf.mapping = my_npm[j].mapping;
				}
			}
		}
	}
	printf("New Mapping ID == %d\n", npf.mapping);

	/*
	* Build protocol string to send
	*/
//	inet_pton(AF_INET6, "fe80::20c:29ff:fe1f:7035", &npf.source);
	npf.source = conf.lladdr;

	if(getaddrinfo(saddr_s, "5555", &req, &ai))
	{
		perror("getaddrinfo");
        exit(1);
	}

    if ((sockfd = socket(PF_INET6, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if(connect(sockfd, ai->ai_addr, ai->ai_addrlen) == -1)
    {
        perror("connect");
        exit(1);
    }

    if ((numbytes = send(sockfd, &npf, sizeof(struct np_finalise), 0)) == -1)
    {
        perror("sendto");
        exit(1);
    }

//quit:
    close(sockfd);

	free(to_send);
	free(saddr_s);
	printf("negotiate()\n");
    return 0;
}
#endif
