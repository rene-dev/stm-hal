#include <iostream>
#include <libusb.h>
#include <time.h>
using namespace std;

void printdev(libusb_device *dev); //prototype of the function

static void Callback(struct libusb_transfer *transfer) 
{ 
     cout << "fertig" << endl << flush;
     //printf("\nGot Data"); 
     //here i save the incoming data 
     //libusb_submit_transfer(transfer); 
} 

int main() {
	libusb_device_handle *dev;
	libusb_context *ctx = NULL; //a libusb session
	libusb_transfer *transfer;
	const unsigned char NUM_ISO_PACKETS=1; 
    unsigned char buffer1[NUM_ISO_PACKETS*64]; 
	int r; //for return values
	ssize_t cnt; //holding number of devices in list
  	unsigned char data[64];
	int j;

  	for(int j=0;j<64;j++) { data[j] = 0; }
    data[0] = 0;

	r = libusb_init(&ctx); //initialize a library session
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
				return 1;
	}
	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

	dev = libusb_open_device_with_vid_pid(ctx, 0xcafe, 0xcafe);
	libusb_set_configuration(dev, 0);
  	libusb_claim_interface(dev, 0);
  	
  	transfer=libusb_alloc_transfer(NUM_ISO_PACKETS);
  	if(transfer == NULL)
  	     cout << "error" << endl;
  	transfer->num_iso_packets = NUM_ISO_PACKETS;
  	transfer->type = LIBUSB_TRANSFER_TYPE_BULK;
  	libusb_set_iso_packet_lengths(transfer,64);
  	libusb_fill_bulk_transfer(transfer, dev, 0x01, buffer1, sizeof(buffer1), Callback, NULL, 0);
  	for(int y = 0;y<1;y++)
  	     libusb_submit_transfer(transfer);
  	//sleep(1);
  	//libusb_cancel_transfer(transfer);
  	sleep(1);
  	libusb_free_transfer(transfer);
  	sleep(1);


  float times = 10000;
  time_t t = time(0);
  for(int i = 0; i < times; i++){
	//libusb_control_transfer(dev, 0x40, 0x00, 0, 0, data, sizeof(data), 0);
    //libusb_bulk_transfer(dev, 0x01, data, sizeof(data), &j, 0);
    //cout << "j:" << j << endl;
    //libusb_iso_transfer(dev, 0x83, data, sizeof(data), &i, 0);
    data[0] = 255 - data[0];
  }
  time_t t2 = time(0);
  cout << (float)(t2 - t) << "s" << endl << flush;
  cout << (float)((t2 - t) / times) << "ms / transfer" << endl << flush;



/**/

	libusb_release_interface(dev, 0);
	libusb_close(dev);
	libusb_exit(ctx); //close the session
	return 0;
}