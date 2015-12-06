include ../../Makefile.variables
include ../Makefile.inc

API=-I../../API -I./include
LIBRARIES=-L../syscalls/lib

all: libc

libc:
	$(GCC) $(GCCFLAGS) $(API) $(LIBRARIES) -lsyscalls -c libc.c
	$(AR) rcs lib/libc.a libc.o

clean:
	rm -rf *.o liblib*.a

.PHONY: all clean
