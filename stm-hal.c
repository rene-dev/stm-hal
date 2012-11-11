#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/usb/usbd.h>

int k = 0;
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
//
static const struct usb_endpoint_descriptor data_endp[] = {
    {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x01,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 1,
    }
};
//
const struct usb_interface_descriptor iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 1,
	.bInterfaceClass = 0xFF,
	.bInterfaceSubClass = 0,
	.bInterfaceProtocol = 0,
	.iInterface = 0,
	.endpoint = data_endp,
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
	
	

    //if(buf[0][0] == 0x00)
    //    gpio_set(GPIOD, GPIO12);
    //if(buf[0][0] == 0xff)
    //     gpio_clear(GPIOD, GPIO12);

	return 1;
}

static void cdcacm_data_rx_cb(usbd_device *usbd_dev, u8 ep)
{
	(void)ep;
	gpio_toggle(GPIOD, GPIO12);

	char buf[64];
	int len = usbd_ep_read_packet(usbd_dev, 0x01, buf, 64);
    //gpio_toggle(GPIOD, GPIO14);
    k++;
	//if (len) {
	//	while (usbd_ep_write_packet(usbd_dev, 0x82, buf, len) == 0)
	//		;
	//}

	
}

static void cdcacm_set_config(usbd_device *usbd_dev, u16 wValue)
{
	(void)wValue;
	gpio_toggle(GPIOD, GPIO13);
	usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb);		
	usbd_register_control_callback(
				usbd_dev,
				USB_REQ_TYPE_VENDOR,
				USB_REQ_TYPE_TYPE,
				simple_control_callback);
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
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14);

	usbd_dev = usbd_init(&otgfs_usb_driver, &dev, &config, usb_strings);
	usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);
	

	while (1){
		usbd_poll(usbd_dev);
	   if(k == 10000){
	       gpio_set(GPIOD, GPIO14);
	   }
	}
}

