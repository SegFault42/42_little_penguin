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

static int	dev_open(struct inode *, struct file *);
static int	dev_release(struct inode *, struct file *);
static ssize_t	dev_write(struct file *, const char *, size_t, loff_t *);
static ssize_t	dev_read(struct file *, char *, size_t, loff_t *);

static struct miscdevice	misc_device;
static int			times = 0;
static char			msg[100] = {0};
static short			readPos = 0;

static struct file_operations	fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

static int	dev_open(struct inode *inod, struct file *fi)
{
	times++;
	pr_info("Device opened %d times\n", times);
	return 0;
}

static int	dev_release(struct inode *inod, struct file *fi)
{
	pr_info("Device closed\n");
	return 0;
}
static ssize_t	dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	short	count = 0;

	while (len && (msg[readPos] != 0))
	{
		put_user(msg[readPos], buff++);
		count++;
		len --;
		readPos++;
	}
	return count;
}

static ssize_t	dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	short	ind = len -1;
	short	count = 0;

	memset(msg, 0, 100);
	readPos = 0;
	while (len > 0)
	{
		msg[count++] = buff[ind--];
		len--;
	}
	return count;
}

int	create_misc(void)
{
	int			retval;
	struct file_operations	fops;

	misc_device.minor = MISC_DYNAMIC_MINOR;
	misc_device.name = "fortytwo";
	misc_device.fops = &fops;
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
