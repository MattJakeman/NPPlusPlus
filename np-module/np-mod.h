#include "uthash.h"

#define EXPORT_SYMTAB
/*
* Maximum number of allowed physical mappings
*/
#define NP_PHYS_MAX 256

/*
* NP++ default physical mapping identifier
*/
#define NP_PHYS_DEFAULT 0

/*
* NP++ unsupported physical mapping identifier
*/
#define NP_PHYS_UNSUPPORTED 1

/*
* Physical mapping types
*/
#define NP_TYPE_1_2_1 // One host, one client, eg a router to router link
#define NP_TYPE_1_2_M // One host, many clients, eg a WLAN AP

/*
* Proc stuff
*/
#define PROC_BUF_SIZE 16

/*
* Proc stuff
*/
#define MODE_BUF_SIZE 16

#define DEBUG

/*
* Max length of a MAC address as a string
*/
#define MAX_ADDR_STR_LEN 3 * MAX_ADDR_LEN

/*
* Config mode defines
*/
#define NP_MODE_CLIENT 0
#define NP_MODE_SERVER 1

/*
* Interface modes
*/
#define NP_IF_MODE_CLIENT 0
#define NP_IF_MODE_SERVER 1

/*
* Configuration Struct
*/
struct np_conf
{
	uint8_t icmp6_all_def_map;
	uint8_t debug_on;
};

/*
 * Random Macros
 */
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

extern struct np_conf conf;

/*
* Stats struct
*/
struct npstats
{
	long p_sent;
	long p_recvd;
	long p_in_dropped;
	long p_out_dropped;
};

extern struct npstats np_stats;

/*
* Proc Directory Entries
*/
extern struct proc_dir_entry *np_pde;
extern struct proc_dir_entry *np_ifcurmapping_pde;
extern struct proc_dir_entry *np_maccurmapping_pde;
extern struct proc_dir_entry *np_stats_pde;
extern struct proc_dir_entry *np_conf_pde;
extern struct proc_dir_entry *np_conf_if_modes_pde;

struct np_int_map
{
	struct list_head npil;
	struct proc_dir_entry *int_pde;
	struct proc_dir_entry *if_mode_pde;
	char name[IFNAMSIZ];
	struct net_device *ndp;
	uint8_t mode;
	u_char map;
};

extern struct np_int_map ifcm_pdes;

struct np_mac_map
{
	char mac_str[MAX_ADDR_STR_LEN];
	uint8_t *mac;
	uint8_t mac_len;
	struct proc_dir_entry *mm_pde;
	uint8_t map;
	/*
	* timespec struct to record the time of the last packet from this MAC
	*/
	struct timespec last_used;
	/*
	* Needed by uthash.h for the hash table
	*/
	UT_hash_handle hh;
};

/*
* Hash table pointer for NP++ MAC Mappings
*/
extern struct np_mac_map *npmm_ht;

struct np_sw
{
    struct list_head npswl; // Used for linked list
    u_char phy_id; // ID of the mapping
    const char *desc; // Textual Description
//	uint8_t type; //Mapping Type
/*
* Mapping Types
*/
//#define NP_MAP_TYPE_121 0
//#define NP_MAP_TYPE_12M 1

    int (*phy_input) (struct sk_buff **, const struct net_device *); // Input Function
    struct sk_buff *(*phy_output) (const struct net_device *, struct sk_buff *); // Output function
    void (*phy_slowtimo) (void) ; // Timeout Function
    /*
    * Counters
    */
    long p_sent;   //Packets Sent
    long p_recvd;  //Packets Received
    long p_in_dropped; //Incoming Packets Dropped
    long p_out_dropped; //Outgouing Pacjets Dropped
};

extern struct np_sw np_phys_sw;
/*
* NP++ Functions
*/
unsigned int np_init(void);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
unsigned int np_input(unsigned int hook, struct sk_buff *pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *));
#else
unsigned int np_input(unsigned int hook, struct sk_buff **pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *));
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
unsigned int np_output(unsigned int hook, struct sk_buff *pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *));
#else
unsigned int np_output(unsigned int hook, struct sk_buff **pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *));
#endif

int timeout_thread(void *data);
void init_conf(void);

/*
* np-procfs.c functions
*/
int np_procfs_init(void);
int np_procfs_cleanup(void);
int ifcurmapping_write(struct file *filp, const char __user *buff, unsigned long len, void *data);
int ifcurmapping_read(char *page, char **start, off_t off, int count, int *eof, void *data);
int if_mode_write(struct file *filp, const char __user *buff, unsigned long len, void *data);
int if_mode_read(char *page, char **start, off_t off, int count, int *eof, void *data);
int read_all_stats(char *page, char **start, off_t off, int count, int *eof, void *data);
int read_maccurmapping(char *page, char **start, off_t off, int count, int *eof, void *data);
int write_maccurmapping(struct file *filp, const char __user *buff, unsigned long len, void *data);
int read_icmp6_all_def_map(char *page, char **start, off_t off, int count, int *eof, void *data);
int write_icmp6_all_def_map(struct file *filp, const char __user *buff, unsigned long len, void *data);
int read_debug_on(char *page, char **start, off_t off, int count, int *eof, void *data);
int write_debug_on(struct file *filp, const char __user *buff, unsigned long len, void *data);

/*
* np-mappings.c functions
*/
int mappings_init(void);
int def_input(struct sk_buff **skb, const struct net_device *netd);
struct sk_buff *def_output(const struct net_device *netd, struct sk_buff *skb);

/*
* API Functions
*/
extern int np_mapping_add(struct np_sw *nps);
extern int np_mapping_del(struct np_sw *nps);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,21)
static inline unsigned char *skb_mac_header(const struct sk_buff *skb)
{
	return skb->mac.raw;
}
#elif  LINUX_VERSION_CODE == KERNEL_VERSION(2,6,23) || LINUX_VERSION_CODE == KERNEL_VERSION(2,6,22) /* XXX: Untested */
static inline unsigned char *skb_mac_header(const struct sk_buff **skb)
{
    return (*skb)->mac.raw;
}
#endif

#ifndef MAC_FMT
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

