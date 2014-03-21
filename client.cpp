#include "client.h"

using namespace dicey;

std::string ip_address;
double prob_loss;
double prob_corrupt;
std::string filename;

struct rdt_packet{
	int seq_num; //sequence number
	char data[PACKET_SIZE]; //data array - 128 bytes
	int checksum;
};

int main(int argc, char* argv[]) {
    ip_address = argc > 1 ? argv[1] : "131.204.14.192";
    prob_loss = argc > 2 ? std::atof(argv[2]) : 0;
    prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0;
    filename = argc > 4 ? argv[4] : "TestFile";

	int skt;
	struct sockaddr_in sktaddr;

    std::cout << "dicey " << ip_address << " " << prob_loss << " " << prob_corrupt << " " << filename << std::endl;

	//create socket    
	if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("cannot create socket");
		return 0;
	}

	
    
    return 0;
}
