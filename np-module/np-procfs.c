#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/kobject.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include "np-mod.h"

#define __NO_VERSION__

struct proc_dir_entry *np_pde;
struct proc_dir_entry *np_ifcurmapping_pde;
struct proc_dir_entry *np_maccurmapping_pde;
struct proc_dir_entry *np_stats_pde;
struct proc_dir_entry *np_conf_pde;
struct proc_dir_entry *np_conf_if_modes_pde;
struct np_int_map ifcm_pdes;

int np_procfs_init(void)
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	int i =0;
#endif
	struct net_device *list_dev;
	struct np_int_map *new_ifcm_pde;
	struct proc_dir_entry *stats_all_pde;
	struct proc_dir_entry *conf_icmp6_all_def_map;
	struct proc_dir_entry *conf_debug_on;


	INIT_LIST_HEAD(&ifcm_pdes.npil);

if(conf.debug_on == 1)
	printk(KERN_INFO "Initialising NP++ sysfs entries\n");

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
	np_pde = proc_mkdir("np++", init_net.proc_net);
#else
	np_pde = proc_mkdir("np++", proc_net);
#endif

	np_ifcurmapping_pde = proc_mkdir("ifcurmappings", np_pde);
	np_maccurmapping_pde = proc_mkdir("maccurmappings", np_pde);
	np_stats_pde = proc_mkdir("stats", np_pde);
	np_conf_pde = proc_mkdir("conf", np_pde);
	np_conf_if_modes_pde = proc_mkdir("modes", np_conf_pde);


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
	for_each_netdev(&init_net, list_dev)
#else
	for(i = 1 ; (list_dev = dev_get_by_index(i)) != NULL ; i++)
#endif
    {
		/*
		* Register /proc ifcurmapping variables - add them to ifcm_pdes linked list
		*/
		/*
		* Create the proc entry and set the data pointer to it for use in the read and write functions
		*/
if(conf.debug_on == 1)
		printk(KERN_INFO "Initialising Mapping For Interface %s\n", list_dev->name);

		new_ifcm_pde = kmalloc(sizeof(struct np_int_map), GFP_KERNEL);
		new_ifcm_pde->int_pde = create_proc_entry(list_dev->name , 0644, np_ifcurmapping_pde);
		new_ifcm_pde->int_pde->data = new_ifcm_pde;
		/*
		* Read and write functions for interface mapping
		*/
		new_ifcm_pde->int_pde->read_proc = ifcurmapping_read;
		new_ifcm_pde->int_pde->write_proc = ifcurmapping_write;
		/*
		* Interface mode pde
		*/
		new_ifcm_pde->if_mode_pde = create_proc_entry(list_dev->name , 0644, np_conf_if_modes_pde);
		new_ifcm_pde->if_mode_pde->data = new_ifcm_pde;
		/*
		* Read and write functions for interface mode
		*/
		new_ifcm_pde->if_mode_pde->read_proc = if_mode_read;
		new_ifcm_pde->if_mode_pde->write_proc = if_mode_write;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
		new_ifcm_pde->int_pde->owner = THIS_MODULE;
		new_ifcm_pde->if_mode_pde->owner = THIS_MODULE;
#endif
		/*
		* Copy the device name to the list entry
		*/
		strcpy(new_ifcm_pde->name, list_dev->name);
		/*
		* Set the net_device in the list entry
		*/
		new_ifcm_pde->ndp = list_dev;
		/*
		* Set the current mapping to 0 (Default Mapping)
		*/
		new_ifcm_pde->map = NP_PHYS_DEFAULT;
		new_ifcm_pde->mode = NP_IF_MODE_CLIENT;
		/*
		* Add new_ifcm_pde to the linked list
		*/
		list_add(&(new_ifcm_pde->npil), &(ifcm_pdes.npil));
    }

	stats_all_pde = create_proc_entry("all" , 0644, np_stats_pde);
	stats_all_pde->read_proc = read_all_stats;

	conf_icmp6_all_def_map = create_proc_entry("icmp6_all_def_map", 0644, np_conf_pde);
	conf_icmp6_all_def_map->read_proc = read_icmp6_all_def_map;
	conf_icmp6_all_def_map->write_proc = write_icmp6_all_def_map;


	conf_debug_on = create_proc_entry("debug_on", 0644, np_conf_pde);
	conf_debug_on->read_proc = read_debug_on;
	conf_debug_on->write_proc = write_debug_on;
	return 0;
}

int np_procfs_cleanup(void)
{
    struct np_int_map *del_ifcm_pde;
    struct list_head *pos, *q;
    struct np_mac_map *npmm;

    /*
    * Clean up each interfaces proc_directory_entries
    */
    list_for_each_safe(pos, q, &ifcm_pdes.npil)
    {
        del_ifcm_pde = list_entry(pos, struct np_int_map, npil);
        remove_proc_entry(del_ifcm_pde->name, np_ifcurmapping_pde);
        remove_proc_entry(del_ifcm_pde->name, np_conf_if_modes_pde);
    }


    /*
    * Cleanup MAC stuff 
    */
    for(npmm = npmm_ht ; npmm != NULL ; npmm = npmm->hh.next)
    {
        remove_proc_entry(npmm->mac_str, np_maccurmapping_pde);
    }

	/*
	* Cleanup Conf Stuff
	*/
	remove_proc_entry("icmp6_all_def_map", np_conf_pde);
	remove_proc_entry("debug_on", np_conf_pde);

	/*
	* Remove Stats proc_directory_entries
	*/
	remove_proc_entry("all", np_stats_pde);

	/*
	* Remove the Directories from proc
	*/
    remove_proc_entry("modes", np_conf_pde);
    remove_proc_entry("stats", np_pde);
    remove_proc_entry("conf", np_pde);
    remove_proc_entry("ifcurmappings", np_pde);
    remove_proc_entry("maccurmappings", np_pde);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
    remove_proc_entry("np++", init_net.proc_net);
#else
    remove_proc_entry("np++", proc_net);
#endif

	return 0;
}

int ifcurmapping_write(struct file *filp, const char __user *buff, unsigned long len, void *data)
{
    struct np_int_map *ifcm_pde = (struct np_int_map *)data ;
    struct np_int_map *list_ifcm_pde;
    struct list_head *pos, *q;
    char from_buf[PROC_BUF_SIZE];

if(conf.debug_on == 1)
    printk(KERN_INFO "ifcurmapping_write() called\n");

    /*
    * Read through linked list to match the list entry
    */
    list_for_each_safe(pos, q, &ifcm_pdes.npil)
    {
        list_ifcm_pde = list_entry(pos, struct np_int_map, npil);
        if(list_ifcm_pde == ifcm_pde)
        {
	  /*
	   * Copy the user input to the current mapping in the list entry that has been matched
	   */
	  if (copy_from_user(from_buf, buff, len))
	  {
	    return -EFAULT;
	  }
	  list_ifcm_pde->map = (u_char)simple_strtol(from_buf, NULL, 10);
	  return len;
        }
    }

	/*
    * No matching interface was found so return 0;
    */
	return 0;
}

/*
* Read the current mapping for a /proc/net/np++/ifcurmappings entry
*/
int ifcurmapping_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	struct np_int_map *ifcm_pde = (struct np_int_map *)data ;
    struct np_int_map *list_ifcm_pde;
    struct list_head *pos, *q;

if(conf.debug_on == 1)
	printk(KERN_INFO "ifcurmapping_read() called\n");

    if (off > 0)
    {
        *eof = 1;
        return 0;
    }

	/*
	* Read through linked list and print out current mapping
	*/
    list_for_each_safe(pos, q, &ifcm_pdes.npil)
    {
        list_ifcm_pde = list_entry(pos, struct np_int_map, npil);
		if(list_ifcm_pde == ifcm_pde)
		{
			len = sprintf(page, "%d\n", ifcm_pde->map);
			*eof = 1;
		    return len;
		}
    }

	/*
	* No matching interface was found so return 0;
	*/
	return 0;
}
/*
* Write the current mode to /proc/net/np++/conf/modes/<interface name>
*/
int if_mode_write(struct file *filp, const char __user *buff, unsigned long len, void *data)
{
    struct np_int_map *ifcm_pde = (struct np_int_map *)data ;
    struct np_int_map *list_ifcm_pde;
    struct list_head *pos, *q;
    char from_buf[PROC_BUF_SIZE];
	uint8_t new_mode;

if(conf.debug_on == 1)
    printk(KERN_INFO "if_mode_write() called\n");

    /*
    * Read through linked list to match the list entry
    */
    list_for_each_safe(pos, q, &ifcm_pdes.npil)
    {
        list_ifcm_pde = list_entry(pos, struct np_int_map, npil);
        if(list_ifcm_pde == ifcm_pde)
        {
			/*
			 * Copy the user input to the current mapping in the list entry that has been matched
			 */
			if (copy_from_user(from_buf, buff, len))
			{
				return -EFAULT;
			}
			new_mode = (uint8_t)simple_strtol(from_buf, NULL, 10);
	        if(new_mode == NP_IF_MODE_CLIENT || new_mode == NP_IF_MODE_SERVER)
				list_ifcm_pde->mode = (u_char)simple_strtol(from_buf, NULL, 10);
			else
			{
				printk(KERN_INFO "Error Writing /proc/net/np++/conf/modes/<interface name> - Unsupported Value Entered");
				return -EINVAL;
			}
			return len;
        }
    }

	/*
    * No matching interface was found so return 0;
    */
	return 0;
}

/*
* Read the current mode from /proc/net/np++/conf/modes/<interface name>
*/
int if_mode_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	struct np_int_map *ifcm_pde = (struct np_int_map *)data ;
    struct np_int_map *list_ifcm_pde;
    struct list_head *pos, *q;

if(conf.debug_on == 1)
	printk(KERN_INFO "if_mode_read() called\n");

    if (off > 0)
    {
        *eof = 1;
        return 0;
    }

	/*
	* Read through linked list and print out current mapping
	*/
    list_for_each_safe(pos, q, &ifcm_pdes.npil)
    {
        list_ifcm_pde = list_entry(pos, struct np_int_map, npil);
		if(list_ifcm_pde == ifcm_pde)
		{
			len = sprintf(page, "%d\n", ifcm_pde->mode);
			*eof = 1;
		    return len;
		}
    }

	/*
	* No matching interface was found so return 0;
	*/
	return 0;
}

int read_all_stats(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
    struct list_head *pos, *q;
    struct np_sw *nps;
	char *cat_str = kmalloc(4192, GFP_KERNEL);

if(conf.debug_on == 1)
	printk(KERN_INFO "read_all_stats() called\n");

	len = sprintf(page, "NP++ All Stats\n\nPackets Sent : %ld\nPackets Received : %ld\nIncoming Packets Dropped : %ld\nOutgoing Packets Dropped : %ld\n\nNP++ Mapping Specific Stats :\n\n", np_stats.p_sent, np_stats.p_recvd, np_stats.p_in_dropped, np_stats.p_out_dropped);

	list_for_each_safe(pos, q, &np_phys_sw.npswl)
	{
		nps = list_entry(pos, struct np_sw, npswl);
		len += sprintf(cat_str, "%s :\n\nPackets Sent : %ld\nPackets Received : %ld\nIncoming Packets Dropped : %ld\nOutgoing Packets Dropped : %ld\n\n", nps->desc, nps->p_sent, nps->p_recvd, nps->p_in_dropped, nps->p_out_dropped);

		strcat(page, cat_str);
	}
	kfree(cat_str);
	*eof = 1;
	return len;
}

/*
* Read the current mac address mapping from /proc/net/np++/maccurmappings
*/
int read_maccurmapping(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	struct np_mac_map *new_npmm;
	struct np_mac_map *npmm;

	npmm = (struct np_mac_map *) data;

	if (off > 0)
	{
		*eof = 1;
		return 0;
	}

	/*
	* Get MAC entry from hash table
	*/
	HASH_FIND_STR(npmm_ht, npmm->mac_str, new_npmm);
	if(new_npmm)
	{
		len = sprintf(page, "%d\n", new_npmm->map);
		*eof = 1;
		return len;
	}
	else
	{
		/* XXX : Something really funky has happened, do some sort of error */
		printk(KERN_INFO "Couldn't find the damn thing\n");
		return 0;
	}

	return 0;
}

/*
* Write the current mac address mapping to /proc/net/np++/maccurmappings
*/
int write_maccurmapping(struct file *filp, const char __user *buff, unsigned long len, void *data)
{
	struct np_mac_map *new_npmm;
	struct np_mac_map *npmm;
    char from_buf[PROC_BUF_SIZE];

	npmm = (struct np_mac_map *) data;

	/*
	* Get MAC entry from hash table
	*/
	HASH_FIND_STR(npmm_ht, npmm->mac_str, new_npmm);
	if(new_npmm)
	{
		if (copy_from_user(from_buf, buff, len))
		{
			return -EFAULT;
		}
		new_npmm->map = (uint8_t)simple_strtol(from_buf, NULL, 10);
		return len;
	}
	else
	{
		/* XXX : Something really funky has happened, do some sort of error */
		printk(KERN_INFO "Couldn't find the damn thing\n");
		return 0;
	}

	return 0;
}

/*
* Read /proc/net/np++/conf/icmp6_all_def_map (This defines whether to use the default mapping for all ICMPv6 traffic or just Multicast) (0 for just multicast or 1 for all)
*/
int read_icmp6_all_def_map(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", conf.icmp6_all_def_map);
	return len;
}

/*
* Write /proc/net/np++/conf/icmp6_all_def_map (This defines whether to use the default mapping for all ICMPv6 traffic or just Multicast) (0 for just multicast or 1 for all)

*/
int write_icmp6_all_def_map(struct file *filp, const char __user *buff, unsigned long len, void *data)
{
	char from_buf[MODE_BUF_SIZE];

	if (copy_from_user(from_buf, buff, len))
	{
		return -EFAULT;
	}
	else
	{
		uint8_t new_icmp6_all_def_map = (uint8_t)simple_strtol(from_buf, NULL, 10);
		if(new_icmp6_all_def_map == 0 || new_icmp6_all_def_map == 1)
	    	conf.icmp6_all_def_map = new_icmp6_all_def_map;
		else
		{
			printk(KERN_INFO "Error Writing /proc/net/np++/conf/icmp6_all_def_map - Unsupported Value Entered, Must be 0 or 1");
			return -EINVAL;
		}
	    return len;
    }
	return 0;
}

int read_debug_on(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", conf.debug_on);
	return len;
}

int write_debug_on(struct file *filp, const char __user *buff, unsigned long len, void *data)
{
    char from_buf[MODE_BUF_SIZE];

    if (copy_from_user(from_buf, buff, len))
    {
        return -EFAULT;
    }
    else
    {
        uint8_t new_debug_on = (uint8_t)simple_strtol(from_buf, NULL, 10);
        if(new_debug_on == 0 || new_debug_on == 1)
            conf.debug_on = new_debug_on;
        else
        {
            printk(KERN_INFO "Error Writing /proc/net/np++/conf/debug_on - Unsupported Value Entered, Must be 0 or 1");
            return -EINVAL;
        }
        return len;
    }
	return 0;
}
