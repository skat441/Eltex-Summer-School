#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/uaccess.h>

#define SYSFS_ATTR_NAME "leds_module"
#define BLINK_DELAY HZ/5
#define RESTORE_LEDS 0xFF

static struct kobject *kbleds_kobject;
static struct timer_list my_timer;
static struct tty_driver *my_driver;
static unsigned int led_mask = 0;
static unsigned int blink_state = 0; 

static void set_leds(unsigned int state)
{
    if (my_driver && my_driver->ops && my_driver->ops->ioctl) {
        (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, state);
    }
}

static void blink_timer_func(struct timer_list *unused)
{
    blink_state = !blink_state;
    if (blink_state) {
        set_leds(led_mask);
    } else {
        set_leds(0);
    }
    
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);
}

static ssize_t mask_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%u\n", led_mask);
}

static ssize_t mask_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    unsigned int new_mask;
    if (kstrtouint(buf, 0, &new_mask) < 0)
        return -EINVAL;
    led_mask = new_mask;
    
    if (blink_state) {
        set_leds(led_mask);
    }
    return count;
}

static struct kobj_attribute mask_attribute = __ATTR(mask, 0664, mask_show, mask_store);

static int __init kbleds_init(void)
{
    int error = 0;
    printk(KERN_INFO "kbleds: initializing\n");
    kbleds_kobject = kobject_create_and_add(SYSFS_ATTR_NAME, kernel_kobj);
    error = sysfs_create_file(kbleds_kobject, &mask_attribute.attr);
    my_driver = vc_cons[fg_console].d->port.tty->driver;
    if (!my_driver) {
        printk(KERN_ERR "kbleds: failed to get tty driver\n");
        kobject_put(kbleds_kobject);
        return -ENODEV;
    }
    timer_setup(&my_timer, blink_timer_func, 0);
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);
    
    return 0;
}

static void __exit kbleds_cleanup(void)
{
    printk(KERN_INFO "kbleds: unloading\n");
    del_timer_sync(&my_timer);
    set_leds(RESTORE_LEDS);
    kobject_put(kbleds_kobject);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Novopashin K. I.");
MODULE_DESCRIPTION("Keyboard LEDs control via sysfs");
module_init(kbleds_init);
module_exit(kbleds_cleanup);