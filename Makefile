# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

client: client.h client.cpp
	$(CC) client.cpp -o client

server: server.cpp server.h
	$(CC) server.cpp -o server

all: client server

clean:
	rm -Rf *.o client
	rm -Rf *.o server
