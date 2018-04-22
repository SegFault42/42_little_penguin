#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me> reviewed by SegFault42");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp,
			  char __user *user,
			  size_t size, loff_t *offs);

static ssize_t myfd_write(struct file *fp,
			  const char __user *user,
			  size_t size, loff_t *offs);

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

char str[PAGE_SIZE];
char *tmp;

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	if (retval == -1) {
		pr_err("misc_register() failure\n");
		return -1;
	}
	pr_info("misc_register() Success !\n");
	return 0;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
	pr_info("misc_deregister() Success !\n");
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t	t, i, len;

	len = strlen(str);
	/*************** * Malloc like a boss ***************/
	tmp = kcalloc(PAGE_SIZE, sizeof(char), GFP_KERNEL);
	if (tmp == NULL)
		return -1;
	for (t = strlen(str) - 1, i = 0; i < len; t--, i++)
		tmp[i] = str[t];
	tmp[i] = 0x0;
	len = simple_read_from_buffer(user, size, offs, tmp, i);
	kfree(tmp);
	return len;
}

ssize_t myfd_write(struct file *fp,
		   const char __user *user,
		   size_t size, loff_t *offs)
{
	ssize_t res = 0;

	if (size > PAGE_SIZE) {
		pr_info("Error message too long");
		return -1;
	}
	memset(&str, 0, PAGE_SIZE);
	res = simple_write_to_buffer(str, size, offs, user, size) + 1;
	// 0x0 = ’\0’
	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
