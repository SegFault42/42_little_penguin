#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/dcache.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>
#include <linux/mount.h>
#include <linux/string.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("proc module");
MODULE_LICENSE("GPL");

#define PROCFS_NAME	"mymounts"
#define BUFF_SIZE	256

char		buffer[BUFF_SIZE] = {0};

static struct	proc_dir_entry	*proc_entry;

static ssize_t	read_proc(struct file *filp, char *buf, size_t len, loff_t *offp);

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = read_proc,
};

static ssize_t	read_proc(struct file *filp, char *buf, size_t len, loff_t *offp)
{
	struct dentry	*curdentry;
	char		buff_entry[256] = {0};

	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child)
	{
		if (curdentry->d_flags & DCACHE_MOUNTED) {
			strcat(buffer, curdentry->d_name.name);
			strcat(buffer, "\t");
			strcat(buffer, dentry_path_raw(curdentry, buff_entry, 256));
			strcat(buffer, "\n");
		}
	}
	if (*offp == 0 && len > 5) {
		if (copy_to_user(buf, buffer, strlen(buffer)) != 0) {
			pr_info("copy_to_user() failure");
			return -1;
		}
		*offp = strlen(buffer);
		return strlen(buffer);
	}
	return 0;

}

static int __init begin(void)
{
	proc_entry = proc_create(PROCFS_NAME, 0, NULL, &fops);
	if (proc_entry == NULL) {
		pr_alert("proc_create() failure\n");
		return -ENOMEM;
	}
	pr_info("proc_create() Success\n");
	return 0;
}

static void __exit end(void)
{
	remove_proc_entry(PROCFS_NAME, NULL);
	pr_info("remove_proc_entry() Success\n");
}

module_init(begin);
module_exit(end);
