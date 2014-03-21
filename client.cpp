#include "client.h"

using namespace dicey;

std::string ip_address;
double prob_loss;
double prob_corrupt;
std::string filename;

int main(int argc, char* argv[]) {
    ip_address = argc > 1 ? argv[1] : "127.0.0.1";
    prob_loss = argc > 2 ? std::atof(argv[2]) : 0;
    prob_corrupt = argc > 3 ? std::atof(argv[3]) : 0;
    filename = argc > 4 ? argv[4] : "TestFile";

    std::cout << "dicey " << ip_address << " " << prob_loss << " " << prob_corrupt << " " << filename << std::endl;
    
    return 0;
}