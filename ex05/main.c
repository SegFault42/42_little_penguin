#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("Misc device module");
MODULE_LICENSE("GPL");

static int	misc_open(struct inode *inode, struct file *filp);
static int	misc_release(struct inode *inode, struct file *filp);

static struct miscdevice	misc_device;
static struct file_operations f_ops = {
	.open = misc_open,
	.release = misc_release,
};
static int	incr = 0;


static int	misc_open(struct inode *inode, struct file *filp)
{
	incr++;
	return 0;
}

static int	misc_release(struct inode *inode, struct file *filp)
{
	incr--;
	return 0;
}

int	create_misc(void)
{
	int			retval;

	misc_device.minor = MISC_DYNAMIC_MINOR;
	misc_device.name = "fortytwo";
	misc_device.fops = &f_ops;
	retval = misc_register(&misc_device);
	if (retval == -1) {
		pr_err("misc_register() failure\n");
		return retval;
	}
	pr_info("New misc device : %s, 10, %i\n", misc_device.name, misc_device.minor);
	return 0;
}

static void __exit	clean_up(void)
{
	misc_deregister(&misc_device);
	pr_info("Cleaning up module.\n");
}

module_init(create_misc);
module_exit(clean_up);
