#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MSG_SIZE 10
#define PRIV_FLAGS 0660
#define PROC_ENTRY_NAME "proc_interatcive"

static int len,temp;
static char *msg;

static ssize_t read_proc(struct file*,char*,size_t,loff_t*);
static ssize_t write_proc(struct file*, const char*,size_t,loff_t*);

 
static ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp )
{
    if(count>temp)
    {
        count=temp;
    }
    temp=temp-count;
    if(copy_to_user(buf,msg, count)){
        return -EFAULT;
    }
    if(count==0)
        temp=len;
 
    return count;
}
 
static ssize_t write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
    if(copy_from_user(msg,buf,count)){
        return -EFAULT;
    }
    len=count;
    temp=len;
    return count;
}
 
static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};
 
 
 
static int __init proc_init (void) {

    proc_create(PROC_ENTRY_NAME,PRIV_FLAGS,NULL,&proc_fops);
    msg=kmalloc(MSG_SIZE*sizeof(char), GFP_KERNEL);
    if(!msg){
        return -ENOMEM;
    }
    
    pr_info("Module intialized\n");
    return 0;
}
 
static void __exit proc_cleanup(void) {
    remove_proc_entry(PROC_ENTRY_NAME,NULL);
    kfree(msg);

    pr_info("Module removed\n");
}
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Novopashin");
module_init(proc_init);
module_exit(proc_cleanup);