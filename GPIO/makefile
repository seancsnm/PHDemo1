CC=g++
CFLAGS=-g -Wall
LIBS=-Llibs -lBBBio -lm
OBJS=FileAccessADC.o
PFLAGS=-std=c++0x

all: main

main: FileAccessADC main.cpp
	$(CC) $(CFLAGS) main.cpp $(OBJS) -o main $(LIBS) $(PFLAGS)

FileAccessADC: FileAccessADC.cpp
	$(CC) $(CFLAGS) -c FileAccessADC.cpp $(PFLAGS)
	
clean:
	rm main *.o
	