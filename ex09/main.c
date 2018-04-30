#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/dcache.h>
#include "/sources/linux-4.15.3/fs/proc/internal.h"

static ssize_t	read_proc(struct file *filp, char *buf, size_t len, loff_t *offp);

static struct	proc_dir_entry	*proc_entry;

static int hello_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Hello proc!\n");
	return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file)
{
	return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = hello_proc_open,
	.read = read_proc,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t	read_proc(struct file *filp, char *buf, size_t len, loff_t *offp)
{
	struct dentry	*curdentry;

	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child)
	{
		if ( curdentry->d_flags & DCACHE_MOUNTED)
			pr_info("%s is mounted", curdentry->d_name.name);
	}
	return 0;
}

static int __init begin(void)
{
	proc_entry = proc_create("mymounts", 0, NULL, &fops);
	if (proc_entry == NULL) {
		pr_info("proc_create() failure\n");
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
