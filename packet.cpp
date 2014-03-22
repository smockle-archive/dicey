class Packet {
    Packet::Packet(bit seq_num, char data[PACKET_SIZE]) {
        this.seq_num = seq_num;
        this.ack = 0;
        this.data = data;
        this.checksum = generate_checksum();
    }
    
    ush Packet::generate_checksum() {
        ush checksum = 1000;
        std::cout << "DEBUG (generate_checksum): " << checksum << std::endl;
        return checksum;
    }

    bool Packet::test_checksum() {
        std::cout << "DEBUG (test_checksum): " << this.checksum == generate_checksum() << std::endl;
        return this.checksum == generate_checksum();
    }
}