#include "server.h"

using namespace dicey;

int main() {
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
			
			//SEND ack, squ_num
			char * response = new char[2];
			response[0] = buffer[0];
			response[1] = '1';
			if(sendto(skt, response, strlen(response), 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
				perror("Unable to send ACK/NAK.");
				return 0;
			}
			else
				std::cout << "message sent";

		}
	}
}