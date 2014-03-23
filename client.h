#ifndef CLIENT_H
#define CLIENT_H

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT_NO 10022
#define PACKET_SIZE 126
#define BUFFER_SIZE 2048

typedef unsigned short ush;
typedef bool bit;

namespace dicey {
    std::string srv_ip_address;
    double prob_loss;
    double prob_corrupt;
    std::string filename;
}

#endif
