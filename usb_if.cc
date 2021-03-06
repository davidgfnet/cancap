
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <string.h>

#define TARGET_VENDOR_ID	0xDEAD  // Put here device VendorID & ProductID
#define TARGET_PRODUCT_ID	0xBEEF

libusb_device_handle * devhandle = NULL;
libusb_context * libusb_ctx = NULL;

extern int global_end;

void start_usb() {
	// discover devices
	int attached = 0;
 
	libusb_init(&libusb_ctx);
	libusb_set_debug(libusb_ctx,3);
	
	// For convenience assume maximum one SBACOM connected
	devhandle = libusb_open_device_with_vid_pid(libusb_ctx, TARGET_VENDOR_ID, TARGET_PRODUCT_ID);
	if (devhandle == NULL) {
		fprintf(stderr, "Could not find a device with vendor/product 0x%x:0x%x\n", TARGET_VENDOR_ID, TARGET_PRODUCT_ID);
		exit(1);
	}
	
	if (libusb_kernel_driver_active(devhandle,0)) {
		fprintf(stderr,"Device seems busy... detaching it ...\n"); 
		libusb_detach_kernel_driver(devhandle,0);
	}
 
	if (libusb_claim_interface( devhandle, 0 )) {
		fprintf(stderr,"Failed to claim interface! Is it busy? Maybe it cannot be detached\n");
		exit(1);
	}
	fprintf(stderr,"Seems like we connected to the device!\n");
}

// Blocking read :D
int read_usb(void * buffer, int maxlen) {
	// Bulk read from endpoint 1
	int read;
	while (!global_end) {
		int r = libusb_bulk_transfer(devhandle, 0x81, (unsigned char *)buffer, 1024, &read, 1000);
		if (r != 0 && r != LIBUSB_ERROR_TIMEOUT)
			return -1;
		if (r == 0)
			break;
	}
	
	return read;
}
// Blocking read :D
int read_usb__(void * buffer, int maxlen) {
	// Bulk read from endpoint 1
	int read;
	if (libusb_bulk_transfer(devhandle, 0x81, (unsigned char *)buffer, 1024, &read, 0) != 0)
		return -1;
	
	return read;
}

int write_usb(void * buffer, int maxlen) {
	// Bulk read from endpoint 1
	int written;
	if (libusb_bulk_transfer(devhandle, 1, (unsigned char *)buffer, maxlen, &written, 0) != 0)
		return -1;
	
	return written;
}

void usb_finalize() {
	libusb_close(devhandle);
	libusb_exit(libusb_ctx);
}


