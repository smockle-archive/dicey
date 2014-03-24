#include "client.h"

using namespace dicey;

int main(int argc, char* argv[]) {
    dicey::srv_ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    dicey::prob_loss = argc > 2 ? std::atof(argv[2]) : 0.2;
    dicey::prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0.2;
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
			gremlin(pkt);
		}
		std::cout << std::endl << std::endl << "DEBUG client main corrupted = " << corrupted << "; lost = " << lost << "; passed = " << passed << std::endl;
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
	{
		std::cout << std::endl << std::endl << "Sending Packet: seq_num = " << myPkt.getSeqNum() << "; ack = " << myPkt.getAck() << "; checksum = " << myPkt.getChecksum() << "; data = " << myPkt.getData() << std::endl;
	}
	return 1;
}

bool dicey::rcvPacket(){
	int rcvPoll = 0;
	struct pollfd ufds;
	time_t timer;

	ufds.fd = skt;
	ufds.events = POLLIN;
	rcvPoll = poll(&ufds, 1, 20);

	if( rcvPoll == -1 ) {
		perror("Unable to poll socket.");
		return 0;
	} 
	else if( rcvPoll == 0 ) {
		perror("Timeout");
		return 0;
	} 
	else {
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

void dicey::gremlin(Packet gremPkt){
	int choice = rand() % 10;
	int corruptChance = (int)(prob_corrupt * 10);
	int lossChance = (int)(prob_loss * 10);
	int passChance = 10 - (corruptChance + lossChance);
	char * options = new char[10];
	for(int cor = 0; cor < corruptChance; cor++){
		options[cor] = CORRUPT_CHOICE;
	}
	for(int los = 0; los < lossChance; los++){
		options[los + corruptChance] = LOSS_CHOICE;
	}
	for(int pas = 0; pas < passChance; pas++){
		options[pas + (lossChance + corruptChance)] = PASS_CHOICE;
	}
	//corruption
	if(options[choice] == CORRUPT_CHOICE){
		corrupted++;
		std::cout << std::endl << "DEBUG gremlin CORRUPTION" << std::endl;
		sendPacket(gremPkt);
	}

	//loss
	else if(options[choice] == LOSS_CHOICE){
		std::cout << std::endl << "DEBUG gremlin LOSS" << std::endl;
		lost++;
	}

	//pass
	else if(options[choice] == PASS_CHOICE){
		std::cout << std::endl << "DEBUG gremlin PASS" << std::endl;
		passed++;
		sendPacket(gremPkt);
	}
	else
		std::cout << std::endl << "DEBUG gremlin TRAP options[choice] = " << options[choice] << std::endl;

	if(!rcvPacket())
			gremlin(gremPkt);	
}