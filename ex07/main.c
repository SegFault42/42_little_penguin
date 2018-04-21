#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("debugfs");
MODULE_LICENSE("GPL");

#define LOGIN "rabougue"
#define LOGIN_LEN 8
#define BUFF_SIZE 128

static char			msg_rcv[BUFF_SIZE] = {0};
static struct miscdevice	misc_device;

static int	create_dir(void);
static int	create_file_id(void);
static int	misc_open(struct inode *inode, struct file *filp);
static int	misc_release(struct inode *inode, struct file *filp);
static ssize_t	misc_read(struct file *filp, char *buffer,
			  size_t length, loff_t *offset);
static ssize_t	misc_write(struct file *filp, const char *buffer,
			   size_t length, loff_t *offset);

int		filevalue;
struct dentry	*dir_ptr, *fileret;
const struct file_operations	f_ops = {
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
	pr_debug("buffer = %s\n", buffer);
	return 0;
}

static ssize_t	misc_write(struct file *filp,
			const char *buffer,
			size_t length,
			loff_t *offset)
{
	unsigned long	retval = 0;

	memset(&msg_rcv, 0, BUFF_SIZE);
	if (length > BUFF_SIZE)
		return -EINVAL;
	retval = copy_from_user(msg_rcv, buffer, length);
	if (retval) {
		pr_err("%ld byte(s) can't be copied !\n", retval);
		return retval;
	}
	pr_info("msg = %s\n", msg_rcv);
}

static int	create_dir(void)
{
	dir_ptr = debugfs_create_dir("fortytwo", NULL);
	if (dir_ptr == NULL) {
		pr_err("Create dir failure\n");
		return -EEXIST;
	} else {
		pr_info("Create dir success\n");
	}
	return 0;
}

static int	create_file_id()
{
	fileret = debugfs_create_file("id", 0666, dir_ptr, NULL, &f_ops);
	if (fileret == NULL)
		pr_info("Create file failure\n");
	else
		pr_info("Create file Success\n");
}

static int	__init begin(void)
{
	create_dir();
	create_file_id();
	/*fileret = debugfs_create_file("jiffies", 0444, dir_ptr, NULL, &f_ops);*/
	/*if (fileret == NULL)*/
		/*pr_info("Create file failure\n");*/
	/*else*/
		/*pr_info("Create file Success\n");*/
	return 0;
}

static void	__exit end(void)
{
	debugfs_remove_recursive(dir_ptr);
}

module_init(begin);
module_exit(end);
