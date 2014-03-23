#include "client.h"
#include "packet.h"

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
	struct hostent *h;
	unsigned char buffer[BUFFER_SIZE];
	bool abp = true;
	
	//SEGMENTATION
	//open file
	std::ifstream dataFile (filename.c_str(), std::ifstream::binary);
	//dataFile.open(filename.c_str(), ifstream::binary);
	if(dataFile.is_open()){
		//get filesize
  		dataFile.seekg (0, dataFile.end);
  		int filesize = dataFile.tellg();
  		dataFile.seekg(0, dataFile.beg);
		//std::cout << "DEBUG (client main): filesize = " << filesize << " bytes" << std::endl;

		if(filesize <= 20){
			dataFile.close();
			perror("File is too small.");
			return 0;
		}

		//calculate how many times to pull out 128 bytes
		int numPackets = 1 + ((filesize - 1)/PACKET_SIZE);
		std::cout << "DEBUG (client main): numPackets = " << numPackets << std::endl;

		//pull out exactly 125 bytes and store in a buffer, (make sure to check for null)
		for(int i = 0; i < numPackets; i++){
			char * pktData= new char[PACKET_SIZE];
			dataFile.read(pktData, PACKET_SIZE);

			Packet pkt(abp = !abp, pktData);
		}
		dataFile.close();
	}
	else{
		std::string err = "Unable to open file " + filename;
		perror(err.c_str());
		return 0;
	}


	std::string msg = "This is a test message.";

    //std::cout << "dicey " << dicey::srv_ip_address << " " << dicey::prob_loss << " " << dicey::prob_corrupt << " " << dicey::filename << std::endl;

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

	if(sendto(skt, msg.c_str(), strlen(msg.c_str()), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0){
		perror("Unable to send message.");
		return 0;
	}

	// bool hasRec = false;
	// while(!hasRec){
	// 	int recvLen;
	// 	recvLen = recvfrom(skt, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&srvaddr, &srvaddrLen);
	// 	std::cout << "Received " << recvLen << " bytes." << std::endl;
	// 	if (recvLen > 0){
	// 		buffer[recvLen] = 0;
	// 		std::cout << "Received message: " << std::endl << buffer << std::endl;
	// 	} 
	// }
    
    return 0;
}
