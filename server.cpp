#include "server.h"

int main() {
	struct sockaddr_in addr1;
	struct sockaddr_in addr2;
	socklen_t addr2Len = sizeof(addr2);
	int rlen;
	int skt;
	unsigned char buffer[BUFFER_SIZE];

	if((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Unable to create socket.");
		return 0;
	}

	memset((char *)&addr1, 0, sizeof(addr1));
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_ANY);
	addr1.sin_port = htons(PORT_NO);

	if (bind(skt, (struct sockaddr *)&addr1, sizeof(addr1)) < 0){
		perror("Unable to bind socket.");
		return 0;
	}

	while(1){
		rlen = recvfrom(skt, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr2, &addr2Len);
		std::cout << "Received " << rlen << " bytes." << std::endl;
		if (rlen > 0){
			buffer[rlen] = 0;
			std::cout << "Received message: " << std::endl << buffer << std::endl;
			std::string ack = "ACK";
			if(sendto(skt, ack.c_str(), strlen(ack.c_str()), 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
				perror("Unable to send message.");
				return 0;
			}
		} 
	}
}