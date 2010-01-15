#include "mapad.h"

int reload_config(void)
{
//  static const char file[] = "mapd.conf";
    FILE *filep = fopen(conf.conf_file, "r");
    no_my_maps = 0;
    struct np_map new_npm;
    char *tok;

    if(filep != NULL)
    {
        char line[128];
        while(fgets(line, sizeof(line), filep) != NULL) //Read a line from the file
        {
			if(line[0] == '#')
			{}
            else if(strncmp(line, "Mode", 4) == 0)
            {
                tok = strtok(line, " ");
                tok = strtok(NULL, "\r\n ");
                if(strncmp(tok, "Server", 6) == 0)
                    conf.mode = MAPD_MODE_SERVER;
                else if(strncmp(tok, "Client", 6) == 0)
                    conf.mode = MAPD_MODE_CLIENT;


//				printf("Mode == %d\n", conf.mode);
            }
            else if(strncmp(line, "Mapping", 7) == 0)
            {
                my_npm = realloc(my_npm, sizeof(my_npm) + sizeof(struct np_map));
                tok = strtok(line, " ");
                tok = strtok(NULL, " ");
                new_npm.mapping = atoi(tok);
                tok = strtok(NULL, " ");
                tok = strtok(NULL, " ");
                new_npm.prio = atoi(tok);
                my_npm[no_my_maps] = new_npm;
                printf("Adding new config mapping : %d with priority %d\n", new_npm.mapping, new_npm.prio);
                no_my_maps++;
            }
            else if(strncmp(line, "Interface", 9) == 0)
            {
				conf.interface = malloc(128);
                tok = strtok(line, " ");
                tok = strtok(NULL, "\r\n ");
                //conf.interface = tok;
				strcpy(conf.interface, tok);
                printf("Adding Interface %s From Config File\n", conf.interface);
				conf.ifindex = if_nametoindex(tok);
            }
            else if(strncmp(line, "LLAddress", 9) == 0)
            {
                tok = strtok(line, " ");
                tok = strtok(NULL, "\r\n ");
                inet_pton(AF_INET6,  tok, &conf.lladdr);
                printf("Adding Link-Local Address %s From Config File\n", tok);
            }
        }
    }
    else
    {
        perror("file : ");
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
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if(conf.mode == MAPD_MODE_CLIENT)
            mapd_client_recv(ih, len, saddr, daddr, iif, hoplimit, sock_fd);
        else if(conf.mode == MAPD_MODE_SERVER)
            mapd_server_recv(ih, len, saddr, daddr, iif, hoplimit, sock_fd);
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
