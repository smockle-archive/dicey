#include "client.h"

using namespace dicey;

int main(int argc, char* argv[]) {
    dicey::srv_ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    dicey::prob_loss = argc > 2 ? std::atof(argv[2]) : 0;
    dicey::prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0;
    dicey::filename = argc > 4 ? argv[4] : "TestFile";

    if(!openSocket())
    	return 0;
	
	std::cout << "PUT " << filename << std::endl << std::endl;

	//SEGMENTATION
	//open file
	std::ifstream dataFile (filename.c_str(), std::ifstream::binary);
	if(dataFile.is_open()){
		//get filesize
  		dataFile.seekg (0, dataFile.end);
  		int filesize = dataFile.tellg();
  		dataFile.seekg(0, dataFile.beg);

		if(filesize <= 20000){
			dataFile.close();
			perror("File is too small.");
			return 0;
		}

		//calculate how many times to pull out 122 bytes
		int numPackets = 1 + ((filesize - 1)/PACKET_DATA_SIZE);
		//std::cout << "DEBUG (client main): numPackets = " << numPackets << std::endl;

		//pull out exactly 122 bytes and store in a buffer, (make sure to check for null)
		for(int i = 0; i < numPackets; i++){
			char * pktData= new char[PACKET_DATA_SIZE];
			dataFile.read(pktData, PACKET_DATA_SIZE);

			Packet pkt(abp = !abp, pktData);
			if(!sendPacket(pkt))
				return 0;
		}
		dataFile.close();
	}
	else{
		std::string err = "Unable to open file " + filename;
		perror(err.c_str());
		return 0;
	}
    
    return 0;
}

bool dicey::openSocket(){
	//create socket    
	if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Unable to create socket.");
		return 0;
	}

	memset((char *)&sktaddr, 0, sizeof(sktaddr));
	sktaddr.sin_family = AF_INET;
	sktaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sktaddr.sin_port = htons(0);

	if (bind(skt, (struct sockaddr *)&sktaddr, sizeof(sktaddr)) < 0){
		perror("Unable to bind socket.");
		return 0;
	}

	memset((char *)&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(PORT_NO);
	inet_pton(AF_INET, srv_ip_address.c_str(), &(srvaddr.sin_addr));

	std::cout << "Server IP: " << inet_ntoa(srvaddr.sin_addr) << ":" << ntohs(srvaddr.sin_port) << std::endl;
	return 1;
}

bool dicey::sendPacket(Packet myPkt){
	char * packetAsArray = myPkt.getPacketAsCharArray();

	if(sendto(skt, packetAsArray, strlen(packetAsArray), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0){
		perror("Unable to send message.");
		return 0;
	}
	else
		std::cout << std::endl << std::endl << "Sending Packet: seq_num = " << myPkt.getSeqNum() << "; ack = " << myPkt.getAck() << "; checksum = " << myPkt.getChecksum() << "; data = " << myPkt.getData() << std::endl;
		if(!rcvPacket()){
			sendPacket(myPkt);
		}
	return 1;
}

bool dicey::rcvPacket(){
	timeout.tv_usec = 20;
	if(setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&timeout,sizeof(struct timeval)) < 0){
		return 0;
	}
	bool hasRec = false;
	while(!hasRec){
		setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&timeout,sizeof(struct timeval));
		int recvLen;
		recvLen = recvfrom(skt, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&srvaddr, &srvaddrLen);
		if (recvLen > 0){
			buffer[recvLen] = 0;
			char ack_nak = buffer[1];
			if (ack_nak == '1')
			{
				std::cout << "Server Message: ACK SEQUENCE_NUM = " << buffer[0] << std::endl << std::endl;
				return 1;
			}
			else{
				std::cout << "Server Message: NAK SEQUENCE_NUM = " << buffer[0] << std::endl << std::endl;
				return 0;
			}
		} 
	}
	return 0;
}

void dicey::gremlin(){

}