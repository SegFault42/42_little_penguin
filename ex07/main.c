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
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/time.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("debugfs");
MODULE_LICENSE("GPL");

#define BUFF_SIZE 128
#define LOGIN "rabougue"

static char			msg_rcv[BUFF_SIZE] = {0};
static char			jiffies_buf[BUFF_SIZE] = {0};

static int	create_dir(void);
/*static void	create_file_id(void);*/
/*static void	create_file_jiffies(void);*/
static void	create_file(char *name, int mode);
static uint8_t	cmp_login(void);

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
	if (*offset == 0) {
		if (!strcmp(filp->f_path.dentry->d_name.name, "jiffies")) {
			pr_info("lol");
			/*strcat(jiffies_buf, "kernel timer");*/
		} else {
			if (copy_to_user(buffer, msg_rcv, strlen(msg_rcv)) != 0) {
				pr_info("copy_to_user() failure");
				return -1;
		}
		*offset = strlen(msg_rcv);
		return strlen(msg_rcv);
		}
	}

	pr_debug("buffer = %s\n", buffer);
	return 0;
}

static uint8_t	cmp_login(void)
{
	if (strcmp(LOGIN, msg_rcv)) {
		pr_info("Invalid\n");
		return -EINVAL;
	}
	pr_info("Valid\n");
	return 1;
}

static ssize_t	misc_write(struct file *filp,
			const char *buffer,
			size_t length,
			loff_t *offset)
{
	unsigned long	retval = 0;
	struct timeval	now;

	memset(&msg_rcv, 0, BUFF_SIZE);
	if (length > BUFF_SIZE)
		return -EINVAL;
	retval = copy_from_user(msg_rcv, buffer, length);
	if (retval) {
		pr_err("%ld byte(s) can't be copied !\n", retval);
		return retval;
	}

	if (!strcmp(filp->f_path.dentry->d_name.name, "id")) {
		if (cmp_login() == 1)
			return length;
		else
			return -EINVAL;
	} else if (!strcmp(filp->f_path.dentry->d_name.name, "jiffies")) {
		do_gettimeofday(&now);
		pr_info("now = %lu\n", now.tv_sec);
	}
	pr_info("msg = %s\n", msg_rcv);
	return (0);
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

/*static void	create_file_id(void)*/
/*{*/
	/*fileret = debugfs_create_file("id", 0666, dir_ptr, NULL, &f_ops);*/
	/*if (fileret == NULL)*/
		/*pr_info("Create file \'id\' failure\n");*/
	/*else*/
		/*pr_info("Create file \'id\' Success\n");*/
/*}*/

/*static void	create_file_jiffies(void)*/
/*{*/

	/*fileret = debugfs_create_file("jiffies", 0444, dir_ptr, NULL, &f_ops);*/
	/*if (fileret == NULL)*/
		/*pr_info("Create file \'jiffies\' failure\n");*/
	/*else*/
		/*pr_info("Create file \'jiffies\' Success\n");*/
/*}*/

static void	create_file(char *name, int mode)
{
	fileret = debugfs_create_file(name, mode, dir_ptr, NULL, &f_ops);
	if (fileret == NULL)
		pr_info("Create file \'id\' failure\n");
	else
		pr_info("Create file \'id\' Success\n");
}

static int	__init begin(void)
{
	create_dir();
	create_file("id", 0666);
	create_file("jiffies", 0444);
	/*create_file_id();*/
	/*create_file_jiffies();*/
	return 0;
}

static void	__exit end(void)
{
	debugfs_remove_recursive(dir_ptr);
}

module_init(begin);
module_exit(end);
