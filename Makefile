# Compiler
CC = g++

# Compiler flags
CFLAGS = -c

dicey: client.h client.cpp
	$(CC) client.cpp -o dicey

clean:
	rm -Rf *.o dicey