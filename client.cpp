#include "client.h"

using namespace dicey;

std::string ip_address
int prob_loss;
int prob_corrupt;
std::string filename;

int main(int argc, char* argv[]) {
    std::cout << "<ip_address> <loss_probability> <corrupt_probability> <filename>" << std::endl;
    std::cout << argc << std::endl;

    ip_address = argc > 1 ? argv[1] : "127.0.0.1";
    prob_loss = argc > 2 ? argv[2] : 0;
    prob_corrupt = argc > 3 ? argv[3] : 0;
    filename = argc > 4 ? argv[4] : "TestFile";

    return 0;
}