
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "packet.h"

FILE * fout;

int main(int argc, char ** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s input output\n",argv[0]);
		exit(1);
	}
	
	// Input interface
	FILE * fin = fopen(argv[1], "rb");
	
	// Output interface
	fout = fopen(argv[2], "wb");
	InitHeader();
	
	std::string chunk;
	while (1) {
		int processed;
		while ((processed = ReceivedPacket(chunk)) >= 0) {
			chunk = chunk.substr(processed);
		}
		
		char temp[1024];
		int read = fread(temp,1,1024,fin);
		temp[read] = 0;
		chunk = chunk + strip_spaces(std::string(temp));
		
		if (read == 0) break; // End!
	}
	
	return 0;
}


