include ../../Makefile.variables
include ../Makefile.inc

MODULE=shellCodeModule.bin
SOURCES=shell.c

API=-I../../API -I../libc/include -I./
LIBRARIES=-L../syscalls/lib -L../libc/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shell.ld $(SOURCES) $(API) $(LIBRARIES) -lc -lsyscalls  -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean