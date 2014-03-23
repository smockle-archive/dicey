#ifndef SERVER_H
#define SERVER_H


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#define PORT_NO 10022
#define BUFFER_SIZE 2048

namespace dicey {
	struct sockaddr_in addr1;
	struct sockaddr_in addr2;
	socklen_t addr2Len = sizeof(addr2);
	int rlen;
	int skt;
	unsigned char buffer[BUFFER_SIZE];
}

#endif