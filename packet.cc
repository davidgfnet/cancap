
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "util.h"

extern FILE * fout;

typedef struct pcap_hdr_s {
        uint32_t magic_number;   /* magic number */
        uint16_t version_major;  /* major version number */
        uint16_t version_minor;  /* minor version number */
        int32_t  thiszone;       /* GMT to local correction */
        uint32_t sigfigs;        /* accuracy of timestamps */
        uint32_t snaplen;        /* max length of captured packets, in octets */
        uint32_t network;        /* data link type */
} pcap_hdr_t;
typedef struct pcaprec_hdr_s {
   uint32_t ts_sec;         /* timestamp seconds */
   uint32_t ts_usec;        /* timestamp microseconds */
   uint32_t incl_len;       /* number of octets of packet saved in file */
   uint32_t orig_len;       /* actual length of packet */
} pcaprec_hdr_t;
#define LINKTYPE_CAN_SOCKETCAN 227

void write_data(void * buffer, int length) {
	fwrite(buffer,1,length,fout);
}

int ReceivedPacket(std::string packet) {
	// Return -1 if the packet is not entirely in the buffer
	if (packet.size() < 16) return -1;

	// Decode packet in stream mode
	unsigned int received_size = hex_to_int(packet.substr(0,8));
	unsigned int num_packet    = hex_to_int(packet.substr(8,8));
	
	if (packet.size()-16 < received_size*2) return -1;
	
	std::string hex_data = packet.substr(16,received_size*2);  // Should be multiple of 2 :)
	unsigned int data_size = (hex_data.size()+1)/2;
	
	// Create a PCAP packet
	pcaprec_hdr_t * pcapp = (pcaprec_hdr_t*)malloc(sizeof(pcaprec_hdr_t) + data_size);
	pcapp->ts_sec = num_packet;
	pcapp->ts_usec = 0;
	pcapp->incl_len = data_size;
	pcapp->orig_len = data_size;
	
	// Copy data from HEX
	unsigned char * pcapp_data = (unsigned char*)&pcapp[1];  // Skip header
	hex_to_byte(hex_data, pcapp_data);
	
	// Write the packet to the underlying file
	write_data(pcapp, sizeof(pcaprec_hdr_t) + data_size);
	
	// Return the amount of bytes processed
	return (4+4+received_size)*2;  // Because Hex encoded
}

void InitHeader() {
	pcap_hdr_s head;
	head.magic_number = 0xa1b2c3d4;
	head.version_major = 2;
	head.version_minor = 4;
	head.thiszone = 0;
	head.sigfigs = 0;
	head.snaplen = 65535;
	head.network = LINKTYPE_CAN_SOCKETCAN;
	
	write_data(&head, sizeof(head));
}

