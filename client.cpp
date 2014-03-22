#include "client.h"

using namespace dicey;

int main(int argc, char* argv[]) {
    dicey::srv_ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    dicey::prob_loss = argc > 2 ? std::atof(argv[2]) : 0;
    dicey::prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0;
    dicey::filename = argc > 4 ? argv[4] : "TestFile";

	int skt;
	struct sockaddr_in sktaddr;
	struct sockaddr_in srvaddr;
	socklen_t srvaddrLen = sizeof(srvaddr);
	int rcvLen;
	struct hostent *h;
	std::string msg = "This is a test message.";
	unsigned char buffer[BUFFER_SIZE];

    std::cout << "dicey " << dicey::srv_ip_address << " " << dicey::prob_loss << " " << dicey::prob_corrupt << " " << dicey::filename << std::endl;

	//create socket    
	if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Unable to create socket.");
		return 0;
	}

	memset((char *)&sktaddr, 0, sizeof(sktaddr));
	sktaddr.sin_family = AF_INET;
	sktaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sktaddr.sin_port = htons(0);

	//bind our socket
	if (bind(skt, (struct sockaddr *)&sktaddr, sizeof(sktaddr)) < 0){
		perror("Unable to bind socket.");
		return 0;
	}

	memset((char *)&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT_NO);
	inet_pton(AF_INET, srv_ip_address.c_str(), &(srvaddr.sin_addr));

	//print out server ip and port for reference
	std::cout << "Server IP: " << inet_ntoa(srvaddr.sin_addr) << ":" << ntohs(srvaddr.sin_port) << std::endl;

	//send message to server
	if(sendto(skt, msg.c_str(), strlen(msg.c_str()), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0){
		perror("Unable to send message.");
		return 0;
	}

	//wait for response from server
	bool hasrcv = false;
	while(!hasrcv){
		rcvLen = recvfrom(skt, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&srvaddr, &srvaddrLen);
		if (rcvLen > 0){
			buffer[rcvLen] = 0;
			std::cout << "Received message: " << std::endl << buffer << std::endl;
		} 
	}
    
    return 0;
}
