#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("Misc device module");
MODULE_LICENSE("GPL");

#define LOGIN "rabougue\n"
#define LOGIN_LEN 9
#define BUFF_SIZE 128

static int	misc_open(struct inode *inode, struct file *filp);
static int	misc_release(struct inode *inode, struct file *filp);
static ssize_t	misc_read(struct file *filp, char *buffer, size_t length, loff_t *offset);
static ssize_t	misc_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);

static char			msg_rcv[BUFF_SIZE] = {0};
static struct miscdevice	misc_device;
static struct file_operations	f_ops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write,
};

static int	misc_open(struct inode *inode, struct file *filp)
{
	pr_info("device opened\n");
	return 0;
}

static int	misc_release(struct inode *inode, struct file *filp)
{
	pr_info("device closed\n");
	return 0;
}

static ssize_t		misc_read(struct file *filp,
			char *buffer,
			size_t length,
			loff_t *offset)
{
	if (*offset == 0 && length > LOGIN_LEN) {
		if (copy_to_user(buffer, LOGIN, LOGIN_LEN) != 0) {
			pr_info("copy_to_user() failure");
			return -1;
		}
		*offset = LOGIN_LEN;
		return LOGIN_LEN;
	}
	pr_info("buffer = %s\n", buffer);
	return 0;
}

static ssize_t	misc_write(struct file *filp,
			const char *buffer,
			size_t length,
			loff_t *offset)
{
	unsigned long	retval = 0;

	if (length > BUFF_SIZE)
		return -1;
	if ((retval = copy_from_user(msg_rcv, buffer, length))) {
		pr_err("%ld byte(s) can't be copied !\n", retval);
		return retval;
	}
	pr_info("msg = %s\n", msg_rcv);
	if (strcmp(LOGIN, msg_rcv))
	{
		pr_info("Invalid\n");
		return -EINVAL;
	}
	pr_info("Valid\n");
	return length;
}

int	create_misc(void)
{
	int	retval;

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
