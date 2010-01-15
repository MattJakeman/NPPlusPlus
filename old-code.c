/*
* Discarded MAC CODE ---------------------------------------------------------
*/

//  rt6 = rt6_lookup(&init_net, &ip6->daddr, &ip6->saddr, outdev->ifindex, 1);

/*  if(!rt6)
        printk(KERN_INFO "rt6 == NULL\n");
    else
    {
        printk(KERN_INFO "dev == %s\n", rt6->rt6i_dev->name);
        //mac = rt6->rt6i_nexthop->ha;
        if(!rt6->rt6i_nexthop->ha)
            printk(KERN_INFO "mac == NULL\n");
        else
        {
            printk(KERN_INFO "MAC Directly From L2 Header %.2x:\n", rt6->rt6i_nexthop->ha[0]);
        }
    }
*/
//  mac = rt6->rt6i_nexthop->ha;
/*
    if(mac)
    {
        printk(KERN_INFO "MAC Directly From L2 Header %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
                mac[0],
                mac[1],
                mac[2],
                mac[3],
                mac[4],
                mac[5]);
    }
*/

//  neigh = ndisc_get_neigh((struct net_device *)outdev, &ip6->daddr);
//  neigh = neigh_lookup(&nd_tbl, &ip6->daddr, (struct net_device *)outdev);

/*
    char hw_addr[MAX_ADDR_STR_LEN];
    uint8_t mac_count = 0;
    char temp_str[4];
*/
/*
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23)
    unsigned char *mac = skb_mac_header(pskb);
#else
    unsigned char *mac = skb_mac_header(*pskb);
#endif
*/


/*
    printk(KERN_INFO "MAC Directly From L2 Header %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
                (unsigned char)mac[0],
                (unsigned char)mac[1],
                (unsigned char)mac[2],
                (unsigned char)mac[3],
                (unsigned char)mac[4],
                (unsigned char)mac[5]);*/
//#endif
/*
    for(mac_count = 0; mac_count < indev->addr_len; mac_count++)
    {
        if(mac_count < (indev->addr_len - 1))
        {
            scnprintf(temp_str, 4, "%.2x:", mac[indev->addr_len + mac_count]);
            strncat(hw_addr, temp_str, strlen(temp_str));
        }
        else
        {
            scnprintf(temp_str, 3, "%.2x", mac[indev->addr_len + mac_count]);
            strncat(hw_addr, temp_str, strlen(temp_str));
        }
    }
#ifdef DEBUG
    printk(KERN_INFO "Output MAC Converted To String == %s\n", hw_addr);
#endif
*/



/*---------------------------------------------------------------------------------------------*/
/* From np-mod.c np_input() default case in switch(icmp6->icmp6_type) inside if(npim->mode == NP_IF_MODE_SERVER) - Now always uses default mapping for ICMPv6
/*
                            struct np_mac_map *npmm;
                            HASH_FIND_STR(npmm_ht, hw_addr, npmm);
#ifdef DEBUG
                            if(npmm)
                                printk(KERN_INFO "MAC Address Found In Hash Table\n");
                            else
                                printk(KERN_INFO "MAC Address NOT Found In Hash Table\n");
#endif
                            list_for_each_safe(pos2, q2, &np_phys_sw.npswl)
                            {
                                nps = list_entry(pos2, struct np_sw, npswl);
                                if(nps->phy_id == npmm->map)
                                {
#ifdef DEBUG
                                   printk(KERN_INFO "Receiving packet using %s with interface in Server mode(ICMPv6 - Not RS or NA)\n", nps->desc);
#endif

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
*/
