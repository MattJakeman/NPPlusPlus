/*
Copyright (c) 2009, Matthew Jakeman
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/version.h>
#include "np-mod.h"

int np_mapping_add(struct np_sw *nps)
{
	/*
	* Initialise counters to 0
	*/
if(conf.debug_on == 1)
	printk(KERN_INFO "NP++: Adding %s\n", nps->desc);

	nps->p_sent = 0;
	nps->p_recvd = 0;
	nps->p_in_dropped = 0;
	nps->p_out_dropped = 0;

	/*
	* XXX : Check physical mapping ID is not already in use
	*/

	list_add(&(nps->npswl), &(np_phys_sw.npswl));

	return 0;
}

int np_mapping_del(struct np_sw *nps)
{
	/*
	* XXX : Write Function
	*/

    struct list_head *pos, *q;
    struct np_sw *npsfl;

	list_for_each_safe(pos, q, &np_phys_sw.npswl)
	{
		npsfl = list_entry(pos, struct np_sw, npswl);
//		if(npsfl == nps)
//		if(memcmp(npsfl, nps, sizeof(struct np_sw)) == 0)
		if(npsfl->phy_id == nps->phy_id)
		{
if(conf.debug_on == 1)
			printk(KERN_INFO "NP++: Deleting %s\n", nps->desc);

			list_del(pos);
//			kfree(npsfl);
		}
	}

	return 0;
}
EXPORT_SYMBOL(np_mapping_add);
EXPORT_SYMBOL(np_mapping_del);
