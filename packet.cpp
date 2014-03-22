#include "packet.h"

using namespace dicey;

class Packet {
    // Order in the sequence of sent packets. (0 or 1)
    bit seq_num;

    // Whether the packet was received successfully/acknowledged. (0/nak or 1/ack)
    bit ack;

    // Checksum based on the packet's data.
    ush checksum;

    // 128 bytes of data.
    char data[PACKET_SIZE];
    
    Packet(bit seq_num, char data[PACKET_SIZE]) {
        this->seq_num = seq_num;
        this->ack = 0;
        std::strcpy(this->data, data);
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