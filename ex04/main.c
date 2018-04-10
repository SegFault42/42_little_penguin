#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>
#include <linux/usb/input.h>

MODULE_AUTHOR("SegFault42 <SegFault42@protonmail.com>");
MODULE_DESCRIPTION("Hello World module");
MODULE_LICENSE("GPL");

static int	__init print_hello(void)
{
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

static void	__exit clean_up(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

/*static struct	usb_device_id	skel_table[] = {*/
	/*{ USB_DEVICE(USB_SKEL_VENDOR_ID, USB_SKEL_PRODUCT_ID) },*/
	/*{ }*/
/*};*/
static struct usb_device_id	skel_table[] = {
{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
        USB_INTERFACE_SUBCLASS_BOOT,
        USB_INTERFACE_PROTOCOL_KEYBOARD) },
{}
};

MODULE_DEVICE_TABLE(usb, skel_table);

module_init(print_hello);
module_exit(clean_up);
