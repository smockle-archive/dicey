#ifndef CLIENT_H
#define CLIENT_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/poll.h>

#include "packet.h"

#define PORT_NO 10022
#define PACKET_SIZE 128
#define PACKET_DATA_SIZE 122
#define BUFFER_SIZE 2048
#define CORRUPT_CHOICE '0'
#define LOSS_CHOICE '1'
#define PASS_CHOICE '2'

typedef unsigned short ush;
typedef bool bit;

namespace dicey {
    std::string srv_ip_address;
    double prob_loss;
    double prob_corrupt;
    std::string filename;
    int skt;
    struct timeval timeout; 
	struct sockaddr_in sktaddr;
	struct sockaddr_in srvaddr;
	socklen_t srvaddrLen = sizeof(srvaddr);
	struct hostent *h;
	unsigned char buffer[BUFFER_SIZE];
	bool abp = true;
	int corrupted = 0;
	int lost = 0;
	int passed = 0;

    bool openSocket();
    bool sendPacket(Packet myPkt);
    bool rcvPacket();
    void gremlin(Packet gremPkt);
}

#endif
