#include "packet.h"

using namespace dicey;

    Packet::Packet(bit seq_no, char newData[PACKET_SIZE]) {
        seq_num = seq_no;
        ack = 0;
        std::strcpy(data, newData);
        checksum = generate_checksum();
        std::cout << std::endl << std::endl << "DEBUG (Packet::Packet): seq_num = " << seq_num << "; ack = " << ack << "; checksum = " << checksum << "; data = " << data << std::endl << std::endl;
    }
    
    ush Packet::generate_checksum() {
        ush byteTotal = 0;
        //std::cout << "DEBUG (generate_checksum): strlen(data) = " << strlen(data) << std::endl;
        for(int i = 0; i < strlen(data); i++){
            byteTotal += data[i];
            std::cout << data[i];
        }
        //std::cout << "end" << std::endl << "DEBUG (generate_checksum): byteTotal = " << byteTotal << std::endl;
        ush divisor = byteTotal/256;
        //std::cout << "DEBUG (generate_checksum): divisor = " << divisor << std::endl;
        ush checksum = byteTotal - (divisor * 256);
        //std::cout << "DEBUG (generate_checksum): " << checksum << std::endl;
        return checksum;
    }

    bool Packet::test_checksum() {
        bool test = checksum == generate_checksum();
        //std::cout << "DEBUG (test_checksum): " << test << std::endl;
        return test;
    }