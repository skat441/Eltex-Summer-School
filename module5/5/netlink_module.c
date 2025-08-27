#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>

#define NETLINK_USER 31
#define PORT 0
#define SEQNUMBER 0
#define FLAGS 0

static struct sock *nl_sk = NULL;

static void hello_nl_recv_msg(struct sk_buff *skb)
{

    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    int msg_size;
    char *msg ;
    int res;

    pr_info("Entering: %s\n", __FUNCTION__);


    nlh = (struct nlmsghdr *)skb->data;
    pr_info("Netlink received msg payload: %s\n", (char *)nlmsg_data(nlh));
    pid = nlh->nlmsg_pid; /*pid of sending process */
    msg = kmalloc(nlh->nlmsg_len * sizeof(char), GFP_KERNEL);
    msg = strcat(msg, "Your msg:");
    msg = strcat(msg, nlmsg_data(nlh));
    msg_size = strlen(msg);

    skb_out = nlmsg_new(msg_size, FLAGS);

    if (!skb_out)
    {

        pr_err("Failed to allocate new skb\n");
        return;

    }
    nlh = nlmsg_put(skb_out, PORT, SEQNUMBER, NLMSG_DONE, msg_size, FLAGS);
    NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);

    if (res < 0)
        pr_info("Error while sending bak to user\n");
}

static struct netlink_kernel_cfg cfg = {
   .groups  = 1,
   .input = hello_nl_recv_msg,
};

static int __init hello_init(void)
{

    pr_info("Entering: %s\n", __FUNCTION__);
    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);

    // nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, 0, hello_nl_recv_msg,
    //                              NULL, THIS_MODULE);
    if (!nl_sk)
    {
        pr_err( "Error creating socket.\n");
        return -10;
    }

    return 0;
}

static void __exit hello_exit(void)
{

    pr_info( "exiting hello module\n");
    netlink_kernel_release(nl_sk);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("Novopashin");
MODULE_LICENSE("GPL");