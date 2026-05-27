#include <usb_names.h>

#define MANUFACTURER_NAME          \
	{                          \
		'T', 'r', 'u','e','F','o','r','m','_','T','e','c','h','n','o','l','o','g','i','e','s'  \
	}
#define MANUFACTURER_NAME_LEN 21

#define PRODUCT_NAME                                                            \
	{                                                                       \
		'K', 'i', 't', 'c', 'h', 'e', 'n','S','i','n','k','_','M','I','T','M','x','4' \
	}
#define PRODUCT_NAME_LEN 20

#define SERIAL_NUMBER                                            \
	{                                                        \
		'_','M','I','T','M','_','K','S','2','0','2','5','x','4'   \
	}
#define SERIAL_NUMBER_LEN 15

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
	2 + MANUFACTURER_NAME_LEN * 2,
	3,
	MANUFACTURER_NAME};

struct usb_string_descriptor_struct usb_string_product_name = {
	2 + PRODUCT_NAME_LEN * 2,
	3,
	PRODUCT_NAME};

struct usb_string_descriptor_struct usb_string_serial_number = {
	2 + SERIAL_NUMBER_LEN * 2,
	3,
	SERIAL_NUMBER};

// #define VENDOR_ID             0x0375
// #define PRODUCT_ID            0x0376