# Makefile
# May, 22/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=-pthread

OBJS= local_peer.o peer.o peers.o status.o socket/socket.o util.o \
file_chunk.o thread.o message.o

all: a.out b.out

a.out: test.o $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o a.out $(OBJS) test.o

b.out: test2.o $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o b.out $(OBJS) test2.o

test.o:
local_peer.o:
peer.o:
peers.o:
status.o:
socket/socket.o:
util.o:
file_chunk.o:
thread.o:
message.o:

.PHONY: clean
clean:
	rm -f *.o *.out *.gch
