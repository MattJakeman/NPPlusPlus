#define NP_PHYS_SAMPLE 10

int sample_input(struct sk_buff **skb, const struct net_device *netd);
struct sk_buff *sample_output(const struct net_device *netd, struct sk_buff *skb);
