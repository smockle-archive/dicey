#ifndef SERVER_H
#define SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "packet.h"

#define PORT_NO 10022
#define PACKET_SIZE 128
#define PACKET_DATA_SIZE 122
#define BUFFER_SIZE 2048

namespace dicey {
	struct sockaddr_in addr1;
	struct sockaddr_in addr2;
	socklen_t addr2Len = sizeof(addr2);
	int rlen;
	int skt;
	bit expectedSeq = 1;
	unsigned char buffer[BUFFER_SIZE];
}

#endif