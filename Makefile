# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

client: packet.o client.h client.cpp
	$(CC) packet.o client.cpp -o client

server: packet.o server.h server.cpp
	$(CC) packet.o server.cpp -o server

packet.o: packet.h packet.cpp
	$(CC) $(CFLAGS) packet.cpp

all: client server

clean:
	rm -Rf *.o client
	rm -Rf *.o server
