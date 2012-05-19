# Makefile
# May, 18/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=
LIBS=-lpthread

OBJS=socket.o
S_OBJS=server.o
C_OBJS=client.o

all: server.out client.out

# Server
server.out:$(S_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) -o server.out $(S_OBJS)

server.o: socket.cpp socket.h server.cpp
	$(CXX) $(CFLAGS) -c socket.cpp server.cpp

# Client
client.out: $(C_OBJS) 
	$(CXX) $(CFLAGS) $(LIBS) $(LDFLAGS) -o client.out $(C_OBJS) 

client.o: client.cpp socket.cpp socket.h
	$(CXX) $(CFLAGS) -c client.cpp

.PHONY: clean
clean:
	rm -f *.o *.out 
