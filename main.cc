
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "util.h"
#include "packet.h"
#include "usb_if.h"
#ifdef __WIN32__
#include <windows.h>
#endif

FILE * fout;
FILE * fin;
FILE * finpipe;

int global_end = 0;

void exit_pid(int s) {
	fprintf(stderr,"Exiting!\n");
	global_end = 1;
}

void * pipe_read(void* args) {
	char temp;
	while (1) {
		fread(&temp,1,1,finpipe);
		write_usb(&temp,1);
		printf("%c",temp);
	}
}

#define ARGV_MODE   1
#define ARGV_INPUT  2
#define ARGV_OUTPUT 3

int main(int argc, char ** argv) {
	signal(SIGINT, exit_pid);
	
	if (argc < 4) {
		fprintf(stderr, "Usage:\n  %s mode input output  * capture from file\n  %s usb output    * capture from usb\n",argv[0],argv[0]);
		exit(1);
	}
	
	int bridge = (strcmp(argv[ARGV_MODE],"bridge")==0);

	// Input interface	
	start_usb();
	char estart [] = "connec\n";
	write_usb(estart, strlen(estart));
	
	// Build-in win32 named pipe creation
	#ifdef __WIN32__
	if (strstr(argv[ARGV_OUTPUT],"\\\\.\\pipe\\") == argv[ARGV_OUTPUT]) {
		CreateNamedPipe (argv[ARGV_OUTPUT], PIPE_ACCESS_OUTBOUND, 
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
			PIPE_UNLIMITED_INSTANCES, 100*1024, 100*1024, 1000, 0);
		
	}
	if (strstr(argv[ARGV_INPUT],"\\\\.\\pipe\\") == argv[ARGV_INPUT]) {
		CreateNamedPipe (argv[ARGV_INPUT], PIPE_ACCESS_INBOUND, 
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
			PIPE_UNLIMITED_INSTANCES, 100*1024, 100*1024, 1000, 0);
		
	}
	#endif
	if (bridge)
		finpipe  = fopen(argv[ARGV_INPUT], "rb");
	
	// Output interface
	fout = fopen(argv[ARGV_OUTPUT], "wb");
	InitHeader();
	
	pthread_t tr;
	pthread_create(&tr, 0, &pipe_read, 0);
	
	std::string chunk;
	while (!global_end) {
		if (bridge) {
			fwrite(chunk.c_str(),1,chunk.size(),fout);
			chunk = "";
		}else{
			int processed;
			while ((processed = ReceivedPacket(chunk)) >= 0) {
				chunk = chunk.substr(processed);
			}
		}
		
		int read;
		char temp[1028];
		read = read_usb(temp, 1024);
		temp[read] = 0;
		printf("%s\n",temp);
		
		if (!bridge)
			chunk = chunk + strip_spaces(std::string(temp));
		else
			chunk = chunk + std::string(temp, read);
		
		if (read == 0) break; // End!
	}
	
	usb_finalize();
	
	return 0;
}


