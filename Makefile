# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

client: client.h client.cpp
	$(CC) client.cpp -o client

clean:
	rm -Rf *.o client
