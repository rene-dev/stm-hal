#include <iostream>
#include <libusb.h>
#include <time.h>
using namespace std;

void printdev(libusb_device *dev); //prototype of the function

int main() {
	libusb_device_handle *dev;
	libusb_context *ctx = NULL; //a libusb session
	int r; //for return values
	ssize_t cnt; //holding number of devices in list
  	unsigned char data[64];
	int i;

  	for(int j=0;j<64;j++) { data[j] = 0; }
    data[0] = 0;

	r = libusb_init(&ctx); //initialize a library session
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
				return 1;
	}
	//libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

	dev = libusb_open_device_with_vid_pid(ctx, 0xcafe, 0xcafe);
  	libusb_claim_interface(dev, 0);

/**/
	time_t t = time(0);
  for(int i = 0; i < 20000; i++){
	libusb_control_transfer(dev, 0x40, 0x00, 0, 0, data, sizeof(data), 0);
    //libusb_bulk_transfer(dev, 0x82, data, sizeof(data), &i, 0);
    //libusb_iso_transfer(dev, 0x83, data, sizeof(data), &i, 0);
    data[0] = 255 - data[0];
  }
  time_t t2 = time(0);
    cout << (float)(t2 - t) / 1 << "s" << endl << flush;
  cout << (float)(t2 - t) / 1 / 20000.0 << "s / transfer" << endl << flush;



/**/

	libusb_release_interface(dev, 0);
	libusb_close(dev);
	libusb_exit(ctx); //close the session
	return 0;
}