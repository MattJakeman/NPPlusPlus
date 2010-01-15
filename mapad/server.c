#include "mapad.h"

//pthread_mutex_t send_mutex;
//static pthread_rwlock_t handler_lock;
//static pthread_t icmp6_listener;

int sock_fd;
static pthread_t icmp6_listener;

int start_server()
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
//    pthread_mutexattr_init(&mattrs);
//    pthread_mutexattr_settype(&mattrs, PTHREAD_MUTEX_NORMAL);
/*    if (pthread_mutex_init(&send_mutex, &mattrs) ||
        pthread_rwlock_init(&handler_lock, NULL) ||
        pthread_create(&icmp6_listener, NULL, icmp6_listen, sock_fd))*/
	if(pthread_create(&icmp6_listener, NULL, icmp6_listen, &sock_fd))
    {
        return -1;
    }
	/*
	* Periodically send out Neighbour Solicitations to ff02::1 containing NP++ option
	*/
	while(1)
	{
		printf("Sending NS\n");
		send_ns(sock_fd, NULL);
		sleep(2);
	}

    return 0;
}

int send_ns(int sock, struct in6_addr *dest)
{
	uint8_t all_hosts_addr[] = {0xff,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	struct sockaddr_in6 addr;
	struct nd_neighbor_solicit *ns;
	struct iovec iov;
	struct cmsghdr *cmsg;
	char chdr[CMSG_SPACE(sizeof(struct in6_pktinfo))];
    struct in6_pktinfo *pkt_info;
	struct msghdr mhdr;
	int i,j = 0;

	unsigned char buff[MSG_SIZE];
    int len = 0;
    int err;

	/*
	* Used when adding the NP++ option to the NS
	*/
	struct np_map_adv npma ;

	if (dest == NULL)
    {
        dest = (struct in6_addr *)all_hosts_addr;
	}

	memset((void *)&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(IPPROTO_ICMPV6);
    memcpy(&addr.sin6_addr, dest, sizeof(struct in6_addr));

    memset(&buff, 0, sizeof(buff));
	ns = (struct nd_neighbor_solicit *) buff;

	ns->nd_ns_type = ND_NEIGHBOR_SOLICIT;
    ns->nd_ns_code  = 0;
    ns->nd_ns_cksum = 0;
	ns->nd_ns_target = *dest;

	len = sizeof(struct nd_neighbor_solicit);

	/*
	* Add NP++ Option
	*/

	npma.type = 62;
	npma.no_maps = 0;

	npma.maps = malloc(no_my_maps * sizeof(struct np_map));

	for(i = 0 ; i < no_my_maps ; i++)
	{
		npma.maps = realloc(npma.maps, (npma.no_maps +1) * sizeof(struct np_map));
		npma.maps[npma.no_maps].mapping = my_npm[i].mapping;
		npma.maps[npma.no_maps].prio = my_npm[i].prio;
//		printf("Mapping == %d Prio == %d\n", my_npm[i].mapping, my_npm[i].prio);
		printf("Advertising Mapping id %d -- Prio == %d\n", my_npm[npma.no_maps].mapping,  my_npm[npma.no_maps].prio);
		npma.no_maps++;
	}

	/*
	* NP++ : XXX : Set the ICMPv6 option length now the number of mappings is known
	*/
	for(j = 8; j / ((sizeof(npma) - sizeof(npma.maps)) + (npma.no_maps * 2)) < 1 ; j+= 8)
	{}
	npma.length = j/8;

	/*
	* NP++ : Copy the ICMPv6 option type, length and no_maps field into the send buffer
	*/
	memcpy(buff + len, &npma, sizeof(npma) - sizeof(npma.maps));
	len += (sizeof(npma) - sizeof(npma.maps));

	/*
	* NP++ : Copy the mapping info into the send buffer
	*/
	for (i = 0 ; i < npma.no_maps ; i++)
	{
		memcpy(buff + len, &npma.maps[i], sizeof(npma.maps[i])) ;
		len += 2 ;
	}

	len += (npma.length * 8) - ((npma.no_maps * 2) + (sizeof(npma) - sizeof(npma.maps)));

	/*
	* ------------------ End Option --------------
	*/

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

	err = sendmsg(sock, &mhdr, 0);

	if (err < 0)
	{
		perror("sendmsg() : ");
	}

	return 0;
}


void mapd_server_recv(const struct icmp6_hdr *ih, ssize_t len, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif, int hoplimit, int sock_fd)
{
    if(ih->icmp6_type == ND_NEIGHBOR_ADVERT)
	{
        struct nd_neighbor_advert *na = (struct nd_neighbor_advert *)ih;
        int optlen = len - sizeof(struct nd_neighbor_advert);
        uint8_t *opt = (uint8_t *)(na + 1);
		uint8_t np_fin_recvd = 0;
		uint8_t source_ll_addr_recvd = 0;
		uint8_t mapping = opt[2];
		char *proc_cat_s = malloc(MAX_PROC_STR_SIZE + 1);
		unsigned char *mac;

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
					mac = opt+2;
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
					np_fin_recvd = 1;
					break;
				}
			}
			optlen -= olen;
            opt += olen;
		}
		/*
		* If we have received a finalisation option and a source link layer address option set the mapping
		*/
		if(np_fin_recvd == 1 && source_ll_addr_recvd == 1)
		{
			printf("NA Received Finalising The Mapping %d - Setting the /proc/ entry\n", mapping);
#ifdef __linux__
			snprintf(proc_cat_s, MAX_PROC_STR_SIZE + 1, "echo %d > /proc/net/np++/ifcurmappings/%s", mapping, conf.interface);
			system(proc_cat_s);
#endif
			free(proc_cat_s);
		}



	}
}


/*
* ------------- Everything below here is just for reference
*/
#if 0
int send_to_neighbours(void)
{
    struct
    {
        struct nlmsghdr n;
        struct ifaddrmsg r;
    } req;

    struct rtattr *rta;
    int status;
    char buf[16384];
    struct nlmsghdr *nlmp;

    int fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);

    /* We use RTM_GETADDR to fetch the ip address from the kernel interface table *
     * So what we do here is pretty simple, we populate the msg structure (req)   *
     * the size of the message buffer is specified to netlink message header, and *
     * flags values are set as NLM_F_ROOT | NLM_F_REQUEST. The request flag must  *
     * be set for all messages requesting the data from kernel. The root flag is  *
     * used to notify the kernel to return the full tabel. Another flag (not used)*
     * is NLM_F_MATCH. This is used to get only speficed entried in the table.    *
     * At the time of writing this program this flag is not implemented in kernel */

    memset(&req, 0, sizeof(req));
    req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
    req.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
    req.n.nlmsg_type = RTM_GETNEIGH;


    /* AF_INET6 is used to signify the kernel to fetch only ipv6 entires.         *
     * Replacing this with AF_INET will fetch ipv4 address table.                 */

    req.r.ifa_family = AF_INET6;

    /* Fill up all the attributes for the rtnetlink header. The code is pretty easy*
     * to understand. The lenght is very important. We use 16 to signify the ipv6  *
     * address. If the user chooses to use AF_INET (ipv4) the length has to be     *
     * RTA_LENGTH(4) */

    rta = (struct rtattr *)(((char *)&req) + NLMSG_ALIGN(req.n.nlmsg_len));
    rta->rta_len = RTA_LENGTH(16);

    /* Time to send and recv the message from kernel */

    status = send(fd, &req, req.n.nlmsg_len, 0);

    if (status < 0)
    {
        perror("send");
        return 1;
    }

    status = recv(fd, buf, sizeof(buf), 0);

    if (status < 0)
    {
        perror("recv");
        return 1;
    }

    if(status == 0)
    {
        printf("EOF\n");
        return 1;
    }

    /* Typically the message is stored in buf, so we need to parse the message to *
     * get the required data for our display. */

    for(nlmp = (struct nlmsghdr *)buf; status > sizeof(*nlmp);)
    {
        char ifnamec[IF_NAMESIZE];
        int len = nlmp->nlmsg_len;
        int req_len = len - sizeof(*nlmp);
        struct ndmsg *ndm;
        struct rtattr *rtatp;
        char *localaddr = malloc(128);

        if (req_len<0 || len>status)
        {
            printf("error\n");
            return -1;
        }

        if (!NLMSG_OK(nlmp, status))
        {
            printf("NLMSG not OK\n");
            return 1;
        }

        ndm = (struct ndmsg *)NLMSG_DATA(nlmp);
        rtatp = (struct rtattr *)IFA_RTA(ndm);

        printf("Index Of Iface= %d\n", ndm->ndm_ifindex);
        inet_ntop(AF_INET6, RTA_DATA(&rtatp[NDA_DST]), localaddr, 128);
        printf("%s ", localaddr);

        if(ndm->ndm_flags == NTF_ROUTER)
        {
            printf("dev %s ", if_indextoname(ndm->ndm_ifindex, ifnamec));
            printf("ROUTER ");
        }

        if(ndm->ndm_state == NUD_REACHABLE)
            printf("REACHABLE\n");

        status -= NLMSG_ALIGN(len);
        nlmp = (struct nlmsghdr*)((char*)nlmp + NLMSG_ALIGN(len));
        free(localaddr);
    }

    return 0;
}
#endif
