#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <iostream>
#include <cstring>

#define PACKET_SIZE 128

typedef unsigned short ush;
typedef bool bit;

namespace dicey {
    class Packet {
        private:
            // Order in the sequence of sent packets. (0 or 1)
            bit seq_num;
            // Whether the packet was received successfully/acknowledged. (0/nak or 1/ack)
            bit ack;
            // Checksum based on the packet's data.
            ush checksum;
            // 128 bytes of data.
            char data[PACKET_SIZE];
        
        public:
            // Constructor
            Packet(bit seq_num, char data[PACKET_SIZE]);
        
            // Create a checksum based on the packet's data.
            ush generate_checksum();
        
            // Verify that the packet's checksum matches a checksum based on the packet's data.
            bool test_checksum();
    };
}

#endif
