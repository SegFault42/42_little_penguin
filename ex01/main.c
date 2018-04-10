#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("Hello World module");
MODULE_LICENSE("GPL");

static int	__init print_hello(void)
{
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

static void __exit clean_up(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(print_hello);
module_exit(clean_up);
