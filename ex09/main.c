#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/dcache.h>
#include "/sources/linux-4.15.3/fs/proc/internal.h"

static struct	proc_dir_entry	*proc_entry;

static const struct file_operations fops = {
	.owner = THIS_MODULE,
};

static ssize_t	read_proc(struct file *filp, char *buf, size_t len, loff_t *offp);

static ssize_t	read_proc(struct file *filp, char *buf, size_t len, loff_t *offp)
{
	return 0;
}

static int __init begin(void)
{
	proc_entry = proc_create("mymounts", 0, NULL, &fops);
	if (proc_entry == NULL) {
		pr_alert("proc_create() failure\n");
		return -ENOMEM;
	}
	pr_info("proc_create() Success\n");
	return 0;
}

static void __exit end(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("remove_proc_entry() Success\n");
}

module_init(begin);
module_exit(end);
