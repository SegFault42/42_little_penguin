#include <linux/module.h>
#include <linux/init.h>

static int	__init print_hello(void)
{
	printk(KERN_INFO "Hello world !\n");
	return (0);
}

static void __exit clean_up(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(print_hello);
module_exit(clean_up);
