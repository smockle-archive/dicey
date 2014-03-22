#include "packet.h"

using namespace dicey;

class Packet::Packet {
    Packet(bit seq_num, char data[PACKET_SIZE]) {
        this->seq_num = seq_num;
        this->ack = 0;
        strcpy(this->data, data);
        this->checksum = this->generate_checksum();
    }
    
    ush generate_checksum() {
        ush checksum = 1000;
        std::cout << "DEBUG (generate_checksum): " << checksum << std::endl;
        return checksum;
    }

    bool test_checksum() {
        bool test = this->checksum == this->generate_checksum();
        std::cout << "DEBUG (test_checksum): " << test << std::endl;
        return test;
    }
};