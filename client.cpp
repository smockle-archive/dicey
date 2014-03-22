#include "client.h"

using namespace dicey;

std::string srv_ip_address;
double prob_loss;
double prob_corrupt;
std::string filename;

struct rdt_packet{
	bool seq_num; // sequence number, 0 or 1
    bool ack; // acknowledgement, 0 is nak and 1 is ack
	int checksum; // checksum,
	char data[PACKET_SIZE]; // array, 128 bytes of data
};

int main(int argc, char* argv[]) {
    srv_ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    prob_loss = argc > 2 ? std::atof(argv[2]) : 0;
    prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0;
    filename = argc > 4 ? argv[4] : "TestFile";

	int skt;
	struct sockaddr_in sktaddr;
	struct sockaddr_in srvaddr;
	struct hostent *h;

    std::cout << "dicey " << srv_ip_address << " " << prob_loss << " " << prob_corrupt << " " << filename << std::endl;

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



	
    
    return 0;
}
