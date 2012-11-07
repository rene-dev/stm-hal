#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/usb/usbd.h>

const struct usb_device_descriptor dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = 0xFF,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0xCAFE,
	.idProduct = 0xCAFE,
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

const struct usb_interface_descriptor iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = 0xFF,
	.bInterfaceSubClass = 0,
	.bInterfaceProtocol = 0,
	.iInterface = 0,
};

const struct usb_interface ifaces[] = {{
	.num_altsetting = 1,
	.altsetting = &iface,
}};

const struct usb_config_descriptor config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0x80,
	.bMaxPower = 0x32,

	.interface = ifaces,
};

const char *usb_strings[] = {
	"x",
	"Black Sphere Technologies",
	"Simple Device",
	"1001",
};

static int simple_control_callback(usbd_device *usbd_dev, struct usb_setup_data *req, u8 **buf,
		u16 *len, void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
	(void)buf;
	(void)len;
	(void)complete;
	(void)usbd_dev;

	if (req->bmRequestType != 0x40)
		return 0; /* Only accept vendor request. */

	//if (req->wValue & 1)
	//	gpio_set(GPIOD, GPIO12);
	//else
	//	gpio_clear(GPIOD, GPIO12);
	
	

    if(buf[0][0] == 0x00)
        gpio_set(GPIOD, GPIO12);
    if(buf[0][0] == 0xff)
        gpio_clear(GPIOD, GPIO12);

	return 1;
}

int main(void)
{
	usbd_device *usbd_dev;

	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
	
	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
    rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);
	rcc_peripheral_enable_clock(&RCC_AHB2ENR, RCC_AHB2ENR_OTGFSEN);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO11 | GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO9 | GPIO11 | GPIO12);


	/* LED output */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

	usbd_dev = usbd_init(&stm32f107_usb_driver, &dev, &config, usb_strings);
	usbd_register_control_callback(
				usbd_dev,
				USB_REQ_TYPE_VENDOR,
				USB_REQ_TYPE_TYPE,
				simple_control_callback);

	while (1)
		usbd_poll(usbd_dev);
}

