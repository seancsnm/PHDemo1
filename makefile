# Notes: several linker library files are generated (eg librange.so) that must
# be installed in the INSTALL directory before the final make can be executed.
# Therefore, suggested order of commands for first time builds is:
# 1-- make install
# 2-- make

INSTALL=/usr/local

STRUCT=Data_Structures/

CC=clang
CFLAGS=-g -Wall
OFLAGS=-c -fPIC -I$(STRUCT)


all: range

#Data Structure(s) Build
range: $(STRUCT)range.cpp $(STRUCT)range.hpp
	$(CC) $(CFLAGS) $(OFLAGS) $(STRUCT)range.cpp -o range.o
	$(CC)  -shared -o librange.so range.o

# Removes all compiled files
clean:
	rm -rf *.so
	rm -rf *.o
	rm -rf ./BBBio/*.o

# Will install everything
install: install-range

# Puts the library files in the right place
install-range: range
	cp librange.so $(INSTALL)/lib
	mkdir -p $(INSTALL)/include/benutil
	cp Data_Structures/*.hpp $(INSTALL)/include/benutil