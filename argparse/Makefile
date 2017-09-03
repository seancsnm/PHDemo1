# Notes: a linker library file is generated (libargparse.so) that must
# be installed in the INSTALL directory before the final make can be executed.
# Therefore, suggested order of commands for first time builds is:
# 1-- make install
# 2-- make

INSTALL=/usr/local

# Defaults
CFLAGS ?= -O3 -g -ggdb
LDFLAGS ?=

BASIC_CFLAGS = -Wall -Wextra -fPIC
BASIC_LDFLAGS = -lm

# We use ALL_* variants
ALL_CFLAGS = $(BASIC_CFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(BASIC_LDFLAGS) $(LDFLAGS)

LIBNAME=libargparse

DYLIBSUFFIX=so
STLIBSUFFIX=a
DYLIBNAME=$(LIBNAME).$(DYLIBSUFFIX)
DYLIB_MAKE_CMD=$(CC) -shared -o $(DYLIBNAME) $(ALL_LDFLAGS)
STLIBNAME=$(LIBNAME).$(STLIBSUFFIX)
STLIB_MAKE_CMD=ar rcs $(STLIBNAME)

# Platform-specific overrides
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S),Darwin)
DYLIBSUFFIX=dylib
DYLIB_MAKE_CMD=$(CC) -shared -o $(DYLIBNAME) $(ALL_LDFLAGS)
endif

all: $(DYLIBNAME) $(STLIBNAME)

OBJS += argparse.o
OBJS += test_argparse.o

$(OBJS): %.o: %.c
	$(CC) -o $*.o -c $(ALL_CFLAGS) $<

$(DYLIBNAME): argparse.o
	$(DYLIB_MAKE_CMD) $^

$(STLIBNAME): argparse.o
	$(STLIB_MAKE_CMD) $^

test: test_argparse
	@echo "###### Unit Test #####"
	@./test.sh

test_argparse: $(OBJS)
	$(CC) $(ALL_CFLAGS) -o $@ $^ $(ALL_LDFLAGS)

clean:
	rm -rf test_argparse
	rm -rf *.[ao]
	rm -rf *.dylib

# Will install everything
install: install-libargparse

# Puts the library files in the right place
install-libargparse: 
	cp $(DYLIBNAME) $(INSTALL)/lib
	mkdir -p $(INSTALL)/include/argparse
	cp *.h $(INSTALL)/include/argparse