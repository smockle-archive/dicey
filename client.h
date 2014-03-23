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

#include "packet.h"

#define PORT_NO 10022
#define PACKET_SIZE 128
#define PACKET_DATA_SIZE 122
#define BUFFER_SIZE 2048

typedef unsigned short ush;
typedef bool bit;

namespace dicey {
    std::string srv_ip_address;
    double prob_loss;
    double prob_corrupt;
    std::string filename;
    int skt;
	struct sockaddr_in sktaddr;
	struct sockaddr_in srvaddr;
	socklen_t srvaddrLen = sizeof(srvaddr);
	struct hostent *h;
	unsigned char buffer[BUFFER_SIZE];
	bool abp = true;

    bool openSocket();
    bool sendPacket(Packet myPkt);
    bool rcvPacket();
}

#endif
