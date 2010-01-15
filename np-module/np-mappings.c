#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/version.h>
#include "np-mod.h"

struct np_sw np_phys_sw;
EXPORT_SYMBOL(np_phys_sw);

int mappings_init(void)
{
	/*
	* Initialise np_sw structs for default and unsupported mappings
	*/
	struct np_sw *default_sw = kmalloc(sizeof(struct np_sw), GFP_KERNEL);
	struct np_sw *unsupported_sw = kmalloc(sizeof(struct np_sw), GFP_KERNEL);

		default_sw->phy_id = NP_PHYS_DEFAULT;
		default_sw->desc = "Default Mapping";
		default_sw->phy_input = def_input;
		default_sw->phy_output = def_output;
		default_sw->phy_slowtimo = NULL;
		default_sw->p_sent = 0;
		default_sw->p_recvd = 0;
		default_sw->p_in_dropped = 0;
		default_sw->p_out_dropped = 0;

		unsupported_sw->phy_id = NP_PHYS_UNSUPPORTED;
		unsupported_sw->desc = "Unsupported Mapping";
		unsupported_sw->phy_input = def_input;
		unsupported_sw->phy_output = def_output;
		unsupported_sw->phy_slowtimo = NULL;
		unsupported_sw->p_sent = 0;
		unsupported_sw->p_recvd = 0;
		unsupported_sw->p_in_dropped = 0;
		unsupported_sw->p_out_dropped = 0;

if(conf.debug_on == 1)
	printk(KERN_INFO "NP++: Initialising Compulsory Physical Mappings\n");

    INIT_LIST_HEAD(&np_phys_sw.npswl);

	/*
	* Add the default mapping and unsupported mapping structs to the mapping linked list (np_phys_sw)
	*/
	list_add(&(default_sw->npswl), &(np_phys_sw.npswl));
	list_add(&(unsupported_sw->npswl), &(np_phys_sw.npswl));

	return 0;
}

int def_input(struct sk_buff **skb, const struct net_device *netd)
{
	return 0;
}

struct sk_buff *def_output(const struct net_device *netd, struct sk_buff *skb)
{
	return skb;
}
