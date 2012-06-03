# Makefile
# May, 22/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=-pthread

OBJS= test.o local_peer.o peer.o peers.o status.o socket/socket.o util.o \
file_chunk.o thread.o

all: a.out

a.out: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJS)

test.o:
local_peer.o:
peer.o:
peers.o:
status.o:
socket/socket.o:
util.o:
file_chunk.o:
thread.o:

.PHONY: clean
clean:
	rm -f *.o *.out *.gch
