#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

#define USAGE_STRING "Usage : mapad -C <path to config file>\n"

#define CMSG_BUF_LEN 256
#define MAX_PKT_LEN  1540
#define MSG_SIZE     4096

#define MAX_PROC_STR_SIZE 512

/*
* Max length of hw address (MAC)
*/
#define HWADDR_MAX 32

/*
* mapad Modes
*/
#define MAPD_MODE_CLIENT 0
#define MAPD_MODE_SERVER 1

int start_server();
int start_client();

struct mapad_conf
{
	u_char mode;
	char *conf_file ;
	char *interface;
	int ifindex;
	struct in6_addr lladdr;
};

extern struct mapad_conf conf;

struct np_map
{
	uint8_t mapping;
	uint8_t prio;
}__attribute__((__packed__));

struct np_finalise
{
	struct in6_addr source;
	uint8_t mapping;
}__attribute__((__packed__));

#define NP_FIN_PAD_SIZE 5

struct np_fin_nd_opt
{
    uint8_t         type;
    uint8_t         length;
	uint8_t			mapping;
	uint8_t			pad[NP_FIN_PAD_SIZE];
}__attribute__((__packed__));

extern struct np_map *my_npm;
extern uint8_t no_my_maps;


/*
* NP++ mapping advertisement struct
*/
struct np_map_conf
{
    uint8_t         mapping;
    uint8_t         prio;
    struct np_map_conf *next;
    uint8_t         adv; //Advertise mapping? 0/1
};

/*
* no_maps defines how many mappings are contained in the advert
* x x x x x x x x maps points to the first of the 'np_map' structs containing mapping and priority
* maps needs to be malloc'd for the number of mappings (accessed using maps[x])
*/
struct np_map_adv
{
    uint8_t         type;
    uint8_t         length;
    uint8_t         no_maps;
    struct np_map   *maps;
}__attribute__((__packed__));


/*
* From client.c
*/
void *icmp6_listen(void *arg);
void mapd_client_recv(const struct icmp6_hdr *ih, ssize_t len, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif, int hoplimit, int sock_fd);
uint8_t negotiate(struct np_map *npm, uint8_t no_maps, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif, int sock_fd);
void init_my_npm();
void *input_thread(void *args);

/*
* From server.c
*/
void *icmp6_create(struct iovec *iov, uint8_t type, uint8_t code);
//int send_to_neighbours(void);
int send_ns(int sock, struct in6_addr *dest);
void mapd_server_recv(const struct icmp6_hdr *ih, ssize_t len, const struct in6_addr *saddr, const struct in6_addr *daddr, int iif, int hoplimit, int sock_fd);

/*
* From utils.c
*/
int reload_config(void);
ssize_t icmp6_recv(int sockfd, unsigned char *msg, size_t msglen, struct sockaddr_in6 *addr, struct in6_pktinfo *pkt_info, int *hoplimit);
