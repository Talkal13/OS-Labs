/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>	/* for copy_to_user */
#include <linux/cdev.h>
#include <linux/module.h>
#include <asm-generic/errno.h>
#include <linux/init.h>
#include <linux/tty.h>      /* For fg_console */
#include <linux/kd.h>       /* For KDSETLED */
#include <linux/vt_kern.h>
#include <linux/version.h> /* For LINUX_VERSION_CODE */

MODULE_LICENSE("GPL");

/*
 *  Prototypes
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80		/* Max length of the message from the device */

/*
 * Global variables are declared as static, so are global within the file.
 */


#define ALL_LEDS_ON 0x7
#define ALL_LEDS_OFF 0


struct tty_driver* kbd_driver= NULL;
dev_t start;
struct cdev* chardev=NULL;
static int Device_Open = 0;	/* Is device open?
				 * Used to prevent multiple access to device */
static char msg[BUF_LEN];	/* The msg the device will give when asked */
static char *msg_Ptr;		/* This will be initialized every time the
				   device is opened successfully */
static int counter=0;		/* Tracks the number of times the character
				 * device has been opened */

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/* Get driver handler */
struct tty_driver* get_kbd_driver_handler(void)
{
    printk(KERN_INFO "modleds: loading\n");
    printk(KERN_INFO "modleds: fgconsole is %x\n", fg_console);
#if ( LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32) )
    return vc_cons[fg_console].d->port.tty->driver;
#else
    return vc_cons[fg_console].d->vc_tty->driver;
#endif
}

/* Set led state to that specified by mask */
static inline int set_leds(struct tty_driver* handler, unsigned int mask)
{
#if ( LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32) )
    return (handler->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,mask);
#else
    return (handler->ops->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, mask);
#endif
}


/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
    int major;		/* Major number assigned to our device driver */
    int minor;		/* Minor number assigned to the associated character device */
    int ret;

    /* Get available (major,minor) range */
    if ((ret=alloc_chrdev_region (&start, 0, 1,DEVICE_NAME))) {
        printk(KERN_INFO "Can't allocate chrdev_region()");
        return ret;
    }

    /* Create associated cdev */
    if ((chardev=cdev_alloc())==NULL) {
        printk(KERN_INFO "cdev_alloc() failed ");
        unregister_chrdev_region(start, 1);
        return -ENOMEM;
    }

    cdev_init(chardev,&fops);

    if ((ret=cdev_add(chardev,start,1))) {
        printk(KERN_INFO "cdev_add() failed ");
        kobject_put(&chardev->kobj);
        unregister_chrdev_region(start, 1);
        return ret;
    }

    major=MAJOR(start);
    minor=MINOR(start);

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'sudo mknod -m 666 /dev/%s c %d %d'.\n", DEVICE_NAME, major,minor);
    printk(KERN_INFO "Try to cat and echo to the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
    /* Destroy chardev */
    if (chardev)
        cdev_del(chardev);

    /*
     * Unregister the device
     */
    unregister_chrdev_region(start, 1);
}

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/chardev"
 */
static int device_open(struct inode *inode, struct file *file)
{
    if (Device_Open)
        return -EBUSY;

    Device_Open++;

    /* Initialize msg */
    sprintf(msg, "I already told you %d times Hello world!\n", counter++);

    /* Initially, this points to the beginning of the message */
    msg_Ptr = msg;

    /* Increase the module's reference counter */
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;		/* We're now ready for our next caller */

    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
     */
    module_put(THIS_MODULE);

    return 0;
}

/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
                           char *buffer,	/* buffer to fill with data */
                           size_t length,	/* length of the buffer     */
                           loff_t * offset)
{
   printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
    return -EPERM;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/chardev
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    /*Creates the mask from the user input (?)*/
    kbd_driver= get_kbd_driver_handler();
    /*Change the leds acording to the input*/	
    set_leds(kbd_driver,ALL_LEDS_ON);
    return 0;
}


