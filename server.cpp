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

	//open file
	std::ofstream writeStream ("TestFile srvcpy");
	int rcvPkts = 0;
	int srvPkts = 0;

	bit endPkts = 0;
	while(!endPkts){
		rlen = recvfrom(skt, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addr2, &addr2Len);
		if (rlen > 0){
			if (rlen < 128)
				endPkts = 1;
			rcvPkts++;
			buffer[rlen] = 0;
			
			// Create packet from received data
			bit serverSeq = 0;
			if(buffer[0] == '1')
				serverSeq = 1;
			char * fileData = new char[PACKET_DATA_SIZE];
			for(int i = 0; i < PACKET_DATA_SIZE; i++){
				fileData[i] = buffer[i + 6];
			}
			
			Packet serverPkt(serverSeq, fileData);
			char * dataChecksum = new char[sizeof(int)];
			for(int j = 0; j < sizeof(int); j++){
				dataChecksum[j] = buffer[j + 2];
			}

			std::cout << std::endl << std::endl << "Received Packet: seq_num = " << serverPkt.getSeqNum() << "; ack = " << serverPkt.getAck() << "; checksum = " << serverPkt.getChecksum() << "; data = " << serverPkt.getData() << std::endl << std::endl;

			// Calculate checksum based on received data and compare to checksum received from client
			int calcChecksum = atoi(dataChecksum);
			serverPkt.setChecksum(calcChecksum);

			if (serverPkt.test_checksum()){
				if (serverSeq != (expectedSeq = !expectedSeq)){
					// SEND nak, squ_num
					char * nakResponse = new char[2];
					nakResponse[0] = buffer[0];
					nakResponse[1] = '0';
					if(sendto(skt, nakResponse, strlen(nakResponse), 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
						perror("Unable to send NAK.");
						return 0;
					}
					else
						std::cout << std::endl << "NAK sent";
				}
				else{
					srvPkts++;
					//pass, send ack
					// Write data to file
					writeStream.write(fileData, strlen(fileData));
					//std::cout << std::endl << std::endl << "DEBUG PACKET DATA = " << fileData << std::endl;

					// SEND ack, squ_num
					char * ackResponse = new char[2];
					ackResponse[0] = buffer[0];
					ackResponse[1] = '1';
					if(sendto(skt, ackResponse, strlen(ackResponse), 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
						perror("Unable to send ACK.");
						return 0;
					}
					else
						std::cout << std::endl << "ACK sent" << std::endl;
				}
			}
			else{
				//fail, send nak
				std::cout << "Checksums do not match. Sequence number = " << buffer[0] << "." << std::endl;
				// SEND nak, squ_num
				char * nakResponse = new char[2];
				nakResponse[0] = buffer[0];
				nakResponse[1] = '0';
				if(sendto(skt, nakResponse, strlen(nakResponse), 0, (struct sockaddr *)&addr2, sizeof(addr2)) < 0){
					perror("Unable to send NAK.");
					return 0;
				}
				else
					std::cout << std::endl << "NAK sent" << std::endl;
			}
		}
	}

	//close file
	writeStream << '\0';
	writeStream.close();
	std::cout << std::endl << "PUT Successful. Packets received: " << rcvPkts << "; Packets served: " << srvPkts << std::endl; 
}