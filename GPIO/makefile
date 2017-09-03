CC=g++
CFLAGS=-g -Wall
LIBS=-Llibs -lBBBio -lrange -lm
ARGLIBS=-largparse
OBJS=FileAccessADC.o
PFLAGS=-std=c++0x

all: main analogReadTest

main: FileAccessADC main.cpp
	$(CC) $(CFLAGS) main.cpp $(OBJS) -o main $(LIBS) $(PFLAGS)

FileAccessADC: FileAccessADC.cpp
	$(CC) $(CFLAGS) -c FileAccessADC.cpp $(PFLAGS)

analogReadTest: analogReadTest.cpp
	$(CC) $(CFLAGS) analogReadTest.cpp -o analogReadTest $(LIBS) $(ARGLIBS) $(PFLAGS)
	
clean:
	rm main *.o analogReadTest