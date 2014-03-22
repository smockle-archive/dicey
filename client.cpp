#include "client.h"

using namespace dicey;

std::string ip_address;
double prob_loss;
double prob_corrupt;
std::string filename;

struct rdt_packet{
	int seq_num; //sequence number
	int checksum;
	char data[PACKET_SIZE]; //data array - 128 bytes
};

int main(int argc, char* argv[]) {
    ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    prob_loss = argc > 2 ? std::atof(argv[2]) : 0;
    prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0;
    filename = argc > 4 ? argv[4] : "TestFile";

	int skt;
	struct sockaddr_in sktaddr;
	struct hostent *h;

    std::cout << "dicey " << ip_address << " " << prob_loss << " " << prob_corrupt << " " << filename << std::endl;

	//create socket    
	if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Unable to create socket.");
		return 0;
	}

	memset((char *))&sktaddr, sizeof(sktaddr));
	sktaddr.sin_family = AF_INET;
	sktaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sktaddr.sin_port = htons(0);

	if (bind(skt, (struct sockaddr *)&sktaddr, sizeof(sktaddr)) < 0){
		perror("Unable to bind socket.");
		return 0;
	}
	
    
    return 0;
}
