#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/debugfs.h>
#include <asm/errno.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("debugfs");
MODULE_LICENSE("GPL");

static int	create_dir(void);

static struct dentry	*create_dir_ptr = NULL;

static int	create_dir(void)
{
	create_dir_ptr = debugfs_create_dir("fortytwo", NULL);
	if (create_dir_ptr == NULL) {
		pr_info("create dir failure\n");
		return -EEXIST;
	} else {
		pr_warn("create dir success\n");
	}
	return 0;
}

static int	__init begin(void)
{
	create_dir();
	return 0;
}

static void	__exit end(void)
{
	/*debugfs*/
}

module_init(begin);
module_exit(end);
