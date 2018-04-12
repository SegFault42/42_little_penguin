#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("Hello World module");
MODULE_LICENSE("GPL");

static int	__init print_hello(void)
{
	pr_info("Hello world !\n");
	return 0;
}

static void	__exit clean_up(void)
{
	pr_info("Cleaning up module.\n");
}

module_init(print_hello);
module_exit(clean_up);
