#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/netfilter.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netdevice.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <net/ip6_route.h>
#include <linux/ipv6.h>
#include <net/ndisc.h>
#include <linux/version.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include "np-mod.h"

#define DRIVER_AUTHOR "Matthew Jakeman <m.jakeman@lancaster.ac.uk>"
#define DRIVER_DESC   "NP++ Module"

MODULE_LICENSE("GPL");

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

/*
* NP++ Globals
*/
//u_char np_physx[NP_PHYS_MAX];
struct npstats np_stats;
struct np_mac_map *npmm_ht;
struct np_conf conf;

/*
* Netfilter Structs
*/
static struct nf_hook_ops np_in_hook_ops =
{
	.list = { NULL, NULL },
	.hook = np_input,
	.pf = PF_INET6,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,24)
	.hooknum = NF_INET_LOCAL_IN
#else
	.hooknum = NF_IP6_LOCAL_IN
#endif
};

static struct nf_hook_ops np_for_hook_ops =
{
	.list = { NULL, NULL },
	.hook = np_input,
	.pf = PF_INET6,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,24)
	.hooknum = NF_INET_FORWARD
#else
	.hooknum = NF_IP6_FORWARD
#endif
};

static struct nf_hook_ops np_out_hook_ops =
{
	.list = { NULL, NULL },
	.hook = np_output,
	.pf = PF_INET6,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,24)
	.hooknum = NF_INET_POST_ROUTING
#else
	.hooknum = NF_IP6_POST_ROUTING
#endif
};

static void init_np_stats(void)
{
	np_stats.p_sent = 0;
	np_stats.p_recvd = 0;
	np_stats.p_in_dropped = 0;
	np_stats.p_out_dropped = 0;
}

/*
* NP++ Initialisation Function
*/
unsigned int np_init(void)
{
	nf_register_hook(&np_in_hook_ops);
	nf_register_hook(&np_for_hook_ops);
	nf_register_hook(&np_out_hook_ops);
	init_np_stats();
	init_conf();
	np_procfs_init();
	mappings_init();
	npmm_ht = NULL;


/*
	for(i = 0 ; i < NP_PHYS_MAX ; i++)
	{
		np_physx[i] = NP_PHYS_UNSUPPORTED;
	}
*/

	return 0;
}

/*
* NP++ Input Processing
*/
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
unsigned int np_input(unsigned int hook, struct sk_buff *pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *))
#else
unsigned int np_input(unsigned int hook, struct sk_buff **pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *))
#endif
{
	struct np_int_map *npim;
	struct list_head *pos, *q,  *pos2, *q2;
	struct np_sw *nps;
	//	struct ipv6hdr *ip6;
	//	struct icmp6hdr *icmp6;
	struct timespec cur_ts;
	struct np_mac_map *new_npmm;

	/*
	* If interface is in server mode
	* Check if packet type is ICMPv6, if so check sender IP, maybe get mac and add to list of client mappings
	*/
	list_for_each_safe(pos, q, &ifcm_pdes.npil)
	{
		npim = list_entry(pos, struct np_int_map, npil);
		if(npim->ndp && npim->ndp->ifindex == indev->ifindex)
		{
			if(npim->mode == NP_IF_MODE_SERVER) // Interface is in server mode so we check the mapping based on source MAC address
			{
				char hw_addr[MAX_ADDR_STR_LEN];
				uint8_t mac_count = 0;
				char temp_str[4];

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
				unsigned char *mac = skb_mac_header(pskb);
#else
				unsigned char *mac = skb_mac_header(*pskb);
#endif

if(conf.debug_on == 1)
				printk(KERN_INFO "Packet Received On An Interface In Server Mode\n");


//				if(mac[mac[indev->addr_len + 6]] == 0x33 && mac[mac[indev->addr_len + 7]] == 0x33) // Multicast packet so use default mapping
				if(CHECK_BIT(mac[0], 8))// Multicast packet so use default mapping
				{
if(conf.debug_on == 1)
					printk(KERN_INFO "Receiving Multicast Packet In Server Mode\n");

					list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
					{
						nps = list_entry(pos2, struct np_sw, npswl);
						if(nps->phy_id == NP_PHYS_DEFAULT)
						{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
							nps->phy_input(&pskb, indev);
#else
							nps->phy_input(pskb, indev);
#endif
							nps->p_recvd++;
							np_stats.p_recvd++;
							return NF_ACCEPT;
						}
					}
				} // END : if(mac[mac[indev->addr_len + 6]] == 0x33 && mac[mac[indev->addr_len + 7]] == 0x33)


				memset(&hw_addr, 0, MAX_ADDR_STR_LEN);

if(conf.debug_on == 1)
{
				printk(KERN_INFO "Source MAC Len == %d\n", indev->addr_len);
				printk(KERN_INFO "Source MAC Directly From L2 Header %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
				            (unsigned char)mac[indev->addr_len],
				            (unsigned char)mac[indev->addr_len+1],
				            (unsigned char)mac[indev->addr_len+2],
				            (unsigned char)mac[indev->addr_len+3],
				            (unsigned char)mac[indev->addr_len+4],
				            (unsigned char)mac[indev->addr_len+5]);
}

				for(mac_count = 0; mac_count < indev->addr_len; mac_count++)
				{
					if(mac_count < (indev->addr_len - 1))
					{
						//scnprintf(hw_addr, "%.2x:", )mac[indev->addr_len + mac_count]);
						scnprintf(temp_str, 4, "%.2x:", mac[indev->addr_len + mac_count]);
						strncat(hw_addr, temp_str, strlen(temp_str));
					}
					else
					{
						scnprintf(temp_str, 3, "%.2x", mac[indev->addr_len + mac_count]);
						strncat(hw_addr, temp_str, strlen(temp_str));
					}
				}
if(conf.debug_on == 1)
				printk(KERN_INFO "Source MAC Converted To String == %s\n", hw_addr);


				HASH_FIND_STR(npmm_ht, hw_addr, new_npmm);

				if(!new_npmm) // Source MAC address not in Hash Table
				{

if(conf.debug_on == 1)
					printk(KERN_INFO "Source MAC Address Struct Not Found In Hash Table, adding %s\n", hw_addr);

					new_npmm = kmalloc(sizeof(struct np_mac_map), GFP_KERNEL);
					new_npmm->mac = kmalloc(indev->addr_len, GFP_KERNEL);
					strcpy(new_npmm->mac_str, hw_addr);
					memcpy(new_npmm->mac, &mac[indev->addr_len], indev->addr_len);
					new_npmm->mac_len = indev->addr_len;
					new_npmm->mm_pde = NULL;
					new_npmm->map = 0;
					new_npmm->last_used = CURRENT_TIME;
					/*
					 * Create proc_dir_entry and associate read and write functions
					 */
					new_npmm->mm_pde = create_proc_entry(hw_addr, 0644, np_maccurmapping_pde);
					new_npmm->mm_pde->data = new_npmm;
					new_npmm->mm_pde->read_proc = read_maccurmapping;
					new_npmm->mm_pde->write_proc = write_maccurmapping;

					/*
					 * Add new MAC info to the hash table
					 */
					HASH_ADD_STR(npmm_ht, mac_str, new_npmm);

					/*
					 * Get the Current timestamp
					 */
if(conf.debug_on == 1)
					printk(KERN_INFO "Current Time Seconds == %ld , Nanoseconds == %ld\n", new_npmm->last_used.tv_sec, new_npmm->last_used.tv_nsec);

					list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
					{
						nps = list_entry(pos2, struct np_sw, npswl);
						if(nps->phy_id == NP_PHYS_DEFAULT)
						{
if(conf.debug_on == 1)
							printk(KERN_INFO "MAC Wasn't Found So Using Default Mapping\n");

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
							nps->phy_input(&pskb, indev);
#else
							nps->phy_input(pskb, indev);
#endif
							nps->p_recvd++;
							np_stats.p_recvd++;
							return NF_ACCEPT;
						}
					}
				} // END : if(!new_npmm)
				else  // Source MAC address found in hash table
				{
if(conf.debug_on == 1)
					printk(KERN_INFO "MAC Found In List\n");

					list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
					{
						nps = list_entry(pos2, struct np_sw, npswl);
						if(nps->phy_id == new_npmm->map)
						{
							/*
							* Set the time stamp in the hash table struct to the current time
							*/
							new_npmm->last_used = CURRENT_TIME;
if(conf.debug_on == 1)
							printk(KERN_INFO "Receiving packet using %s In Server mode\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
							nps->phy_input(&pskb, indev);
#else
							nps->phy_input(pskb, indev);
#endif
							nps->p_recvd++;
							np_stats.p_recvd++;
							return NF_ACCEPT;
						}
					}
				}
			} // END : if(npim->mode == NP_IF_MODE_SERVER)
			else if(npim->mode == NP_IF_MODE_CLIENT)// Interface is in client mode so we can decide the mapping based on interface
			{
				list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
				{
					nps = list_entry(pos2, struct np_sw, npswl);
					if(nps->phy_id == npim->map)
					{
if(conf.debug_on == 1)
						printk(KERN_INFO "Receiving packet using %s with interface in Client mode\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
						nps->phy_input(&pskb, indev);
#else
						nps->phy_input(pskb, indev);
#endif
						nps->p_recvd++;
						np_stats.p_recvd++;
						return NF_ACCEPT;
					}// END : if(nps->phy_id == npim->map)
				} // END : list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
			} //End : else if(npim->mode == NP_IF_MODE_CLIENT)
		} //End : if(npim->ndp && npim->ndp->ifindex == indev->ifindex)
	} // END : list_for_each_safe(pos, q, &ifcm_pdes.npil)
	np_stats.p_in_dropped++;
	return NF_DROP;
}

/*
* NP++ Output Processing
*/
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
unsigned int np_output(unsigned int hook, struct sk_buff *pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *))
#else
unsigned int np_output(unsigned int hook, struct sk_buff **pskb,
               const struct net_device *indev, const
               struct net_device *outdev, int
               (*okfn)(struct sk_buff *))
#endif
{
	struct np_int_map *npim;
	struct list_head *pos, *q,  *pos2, *q2;
	struct np_sw *nps;
	struct ipv6hdr *ip6;
	struct dst_entry *mac_dst;

//	struct neighbour *neigh;
//	struct rt6_info *rt6;
//	unsigned char *mac;

	/*
	* Get the IPv6 Header Pointer
	*/
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
	ip6 = ipv6_hdr(pskb);
#elif LINUX_VERSION_CODE == KERNEL_VERSION(2,6,23) || LINUX_VERSION_CODE == KERNEL_VERSION(2,6,22)
	ip6 = ipv6_hdr(*pskb); /* XXX : Untested */
#else
	ip6 = (*pskb)->nh.ipv6h;
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
	mac_dst = skb_dst(pskb);
#else
	mac_dst = pskb->dst;
#endif
/*
* skb_dst() function in skbuff.h http://lxr.linux.no/#linux+v2.6.32/include/linux/skbuff.h#L425
* see if this returns the correct dst_entry struct
*/

	/*
	* ICMPv6 - Always use default mapping
	*/

	if(conf.icmp6_all_def_map == 1 && ip6->nexthdr == 58)
	{
		list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
		{
			nps = list_entry(pos2, struct np_sw, npswl);
			if(nps->phy_id == NP_PHYS_DEFAULT)
			{
if(conf.debug_on == 1)
				printk(KERN_INFO "Sending ICMPv6 packet using %s\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
				nps->phy_output(indev, pskb);
#else
				nps->phy_output(indev, *pskb);
#endif
				nps->p_sent++;
				np_stats.p_sent++;
				return NF_ACCEPT;
			}
		}
	}
	/*
	* Not ICMPv6 (with conf.icmp6_all_def_map set) so use mapping pointed to in the MAC proc entry if in server mode or interface proc entry for Clients
	* Except multicast traffic which for now uses the default mapping
	*/
	else
	{
if(conf.debug_on == 1)
{
//#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
//#else
		printk(KERN_INFO "MAC From pskb == %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", mac_dst->neighbour->ha[0],
																			mac_dst->neighbour->ha[1],
																			mac_dst->neighbour->ha[2],
																			mac_dst->neighbour->ha[3],
																			mac_dst->neighbour->ha[4],
																			mac_dst->neighbour->ha[5]);
//#endif
}
		/*
		* Multicast traffic so use default mapping
		*/
		if(mac_dst->neighbour->ha[0] == 0x33 && mac_dst->neighbour->ha[1] == 0x33)
		{
			list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
			{
				nps = list_entry(pos2, struct np_sw, npswl);
				if(nps->phy_id == NP_PHYS_DEFAULT)
				{
if(conf.debug_on == 1)
					printk(KERN_INFO "Sending multicast packet using %s\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
					nps->phy_output(indev, pskb);
#else
					nps->phy_output(indev, *pskb);
#endif
					nps->p_sent++;
					np_stats.p_sent++;
					return NF_ACCEPT;
				}
			}
		}

		/*
		* Non multicast traffic so use either MAC or interface mapping depending on interface mode (client/server)
		*/
		list_for_each_safe(pos, q, &ifcm_pdes.npil)
		{
			npim = list_entry(pos, struct np_int_map, npil);
			if(npim->ndp && npim->ndp->ifindex == outdev->ifindex)
			{
				if(npim->mode == NP_IF_MODE_SERVER) // Interface is in server mode so we check the mapping based on destination MAC address
				{
					struct np_mac_map *npmm;
					char hw_addr[MAX_ADDR_STR_LEN];
					int mac_count = 0;
					char temp_str[4];

					memset(&hw_addr, 0, MAX_ADDR_STR_LEN);

					for(mac_count = 0; mac_count < outdev->addr_len; mac_count++)
					{
						if(mac_count < (outdev->addr_len - 1))
						{
							scnprintf(temp_str, 4, "%.2x:", mac_dst->neighbour->ha[mac_count]);
							strncat(hw_addr, temp_str, strlen(temp_str));
						}
						else
						{
							scnprintf(temp_str, 3, "%.2x", mac_dst->neighbour->ha[mac_count]);
							strncat(hw_addr, temp_str, strlen(temp_str));
						}
					}
if(conf.debug_on == 1)
					printk(KERN_INFO "Destination MAC Converted To String == %s\n", hw_addr);


					HASH_FIND_STR(npmm_ht, hw_addr, npmm);
if(conf.debug_on == 1)
{
					if(npmm)
						printk(KERN_INFO "Destination MAC Address Found In Hash Table\n");
					else
						printk(KERN_INFO "Destination MAC Address NOT Found In Hash Table\n");
}

					if(!npmm)
					{
if(conf.debug_on == 1)
						printk(KERN_INFO "Destination MAC Address Struct Not Found In Hash Table, adding\n");
						npmm = kmalloc(sizeof(struct np_mac_map), GFP_KERNEL);
						npmm->mac = kmalloc(outdev->addr_len, GFP_KERNEL);
						strcpy(npmm->mac_str, hw_addr);
						memcpy(npmm->mac, &mac_dst->neighbour->ha[outdev->addr_len], outdev->addr_len);
						npmm->mac_len = outdev->addr_len;
						npmm->mm_pde = NULL;
						npmm->map = 0;
						/*
						 * Create proc_dir_entry and associate read and write functions
						 */
						npmm->mm_pde = create_proc_entry(hw_addr, 0644, np_maccurmapping_pde);
						npmm->mm_pde->data = npmm;
						npmm->mm_pde->read_proc = read_maccurmapping;
						npmm->mm_pde->write_proc = write_maccurmapping;

						/*
						 * Add new MAC info to the hash table
						 */
						HASH_ADD_STR(npmm_ht, mac_str, npmm);
					}
					/*
					* Send the packet using the MAC Mapping
					*/
					list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
					{
						nps = list_entry(pos2, struct np_sw, npswl);
						if(nps->phy_id == npmm->map)
						{
							/*
							* Set time the mac mapping was last used
							*/
							npmm->last_used = CURRENT_TIME;
if(conf.debug_on == 1)
							printk(KERN_INFO "Sending packet using %s in server mode\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
							nps->phy_output(indev, pskb);
#else
							nps->phy_output(indev, *pskb);
#endif
							nps->p_sent++;
							np_stats.p_sent++;
							return NF_ACCEPT;
						}// End : if(nps->phy_id == npmm->map)
					}// End : list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
				}// End : if(npim->mode == NP_IF_MODE_SERVER)
				else if(npim->mode == NP_IF_MODE_CLIENT) // Interface is in client mode so we check the mapping based on Interface
				{
					list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
					{
						nps = list_entry(pos2, struct np_sw, npswl);
						if(nps->phy_id == npim->map)
						{
if(conf.debug_on == 1)
							printk(KERN_INFO "Sending packet using %s in client mode\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
							nps->phy_output(indev, pskb);
#else
							nps->phy_output(indev, *pskb);
#endif
							nps->p_sent++;
							np_stats.p_sent++;
							return NF_ACCEPT;
						}
					}
				}
				else // Interface is not in client or server mode so be hopeful and use the default mapping
				{
					list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
					{
						nps = list_entry(pos2, struct np_sw, npswl);
						if(nps->phy_id == NP_PHYS_DEFAULT)
						{
if(conf.debug_on == 1)
							printk(KERN_INFO "Sending packet using %s in unknown mode\n", nps->desc);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
							nps->phy_output(indev, pskb);
#else
							nps->phy_output(indev, *pskb);
#endif
							nps->p_sent++;
							np_stats.p_sent++;
							return NF_ACCEPT;
						}
					}
				}
			}
		}
		np_stats.p_out_dropped++;
		return NF_DROP;
	}
	return NF_DROP;
}

int init_module(void)
{
if(conf.debug_on == 1)
	printk(KERN_INFO "Loading NP++ Kernel Module\n");


	/*
	 * A non 0 return means init_module failed; module can't be loaded.
	 */
	np_init();

	kthread_run(timeout_thread, NULL, "NP++ Timeout Thread");

if(conf.debug_on == 1)
	printk(KERN_INFO "Finished Loading NP++ Module\n");

	return 0;
}


void cleanup_module(void)
{
if(conf.debug_on == 1)
	printk(KERN_INFO "Removing NP++ Kernel Module\n");

	/*
	* Unregister Netfilter Hooks
	*/
	nf_unregister_hook(&np_in_hook_ops);
	nf_unregister_hook(&np_for_hook_ops);
	nf_unregister_hook(&np_out_hook_ops);

	np_procfs_cleanup();
if(conf.debug_on == 1)
	printk(KERN_INFO "NP++ Module Succesfully removed\n");
}

int timeout_thread(void *data)
{
	struct np_sw *nps;
	struct list_head *pos, *q;

	while(1)
	{
		set_current_state(TASK_RUNNING);

		list_for_each_safe(pos, q, &np_phys_sw.npswl)
    	{
        	nps = list_entry(pos, struct np_sw, npswl);
			if(nps->phy_slowtimo)
				nps->phy_slowtimo();
		}

		set_current_state(TASK_INTERRUPTIBLE);
		msleep(500);
	}
	return 0;
}

void init_conf(void)
{
	conf.icmp6_all_def_map = 0;
	conf.debug_on = 1;
}
