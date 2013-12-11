
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "packet.h"
#include "usb_if.h"

FILE * fout;
FILE * fin;

int main(int argc, char ** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage:\n  %s input output  * capture from file\n  %s usb output    * capture from usb\n",argv[0],argv[0]);
		exit(1);
	}
	
	int use_usb = (strcmp(argv[1],"usb")==0);

	// Input interface	
	if (use_usb)
		start_usb();
	else
		fin = fopen(argv[1], "rb");
	
	// Output interface
	fout = fopen(argv[2], "wb");
	InitHeader();
	
	std::string chunk;
	while (1) {
		int processed;
		while ((processed = ReceivedPacket(chunk)) >= 0) {
			chunk = chunk.substr(processed);
		}
		
		int read;
		char temp[1028];
		if (use_usb) read = read_usb(temp, 1024);
		else         read = fread(temp,1,1024,fin);
		temp[read] = 0;
		chunk = chunk + strip_spaces(std::string(temp));
		
		if (read == 0) break; // End!
	}
	
	if (use_usb) usb_finalize();
	
	return 0;
}


