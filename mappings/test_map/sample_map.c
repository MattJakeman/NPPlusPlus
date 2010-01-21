#include <linux/module.h>   /* Needed by all modules */
#include <linux/if.h>
#include <linux/skbuff.h>
#include <linux/version.h>
#include <linux/netdevice.h>

#include "../np-mod.h"
#include "sample_map.h"

#define DRIVER_AUTHOR "Matthew Jakeman <m.jakeman@lancaster.ac.uk>"
#define DRIVER_DESC   "NP++ Sample Mapping"

MODULE_LICENSE("GPL");

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

static struct np_sw sample_np_sw;

int init_module(void)
{
	sample_np_sw.phy_id = NP_PHYS_SAMPLE;
    sample_np_sw.desc = "Sample Mapping";
    sample_np_sw.phy_input = sample_input;
    sample_np_sw.phy_output = sample_output;
    sample_np_sw.phy_slowtimo = NULL;
    sample_np_sw.p_sent = 0;
    sample_np_sw.p_recvd = 0;
    sample_np_sw.p_in_dropped = 0;
    sample_np_sw.p_out_dropped = 0;

	np_mapping_add(&sample_np_sw);

	return 0;
}

void cleanup_module(void)
{
	np_mapping_del(&sample_np_sw);
}

int sample_input(struct sk_buff **skb, const struct net_device *netd)
{
	/*
	* This is where the packet enters the system.
	* Add code here to return the packet to the Logical Spec
	*/
    return 0;
}

struct sk_buff *sample_output(const struct net_device *netd, struct sk_buff *skb)
{
	/*
	* This is where the packet exits the system
	* Do whatever you want to the packet here (stored in skb)
	*/
    return skb;
}
