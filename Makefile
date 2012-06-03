# Makefile
# May, 22/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=
LIBS=

OBJS= test.o local_peer.o peer.o peers.o status.o util.o

all: a.out

a.out: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)

test.o:
local_peer.o:
peer.o:
peers.o:
status.o:
util.o:

.PHONY: clean
clean:
	rm -f *.o *.out *.gch
