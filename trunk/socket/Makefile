# Makefile
# May, 18/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=
LIBS=-lpthread

S_OBJS=socket.o server.o
C_OBJS=socket.o client.o

all: server.out client.out

# Server executable
server.out: $(S_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) $(S_OBJS) -o server.out

# Client executable
client.out: $(C_OBJS)
	$(CXX) $(CFLAGS) $(LIBS) $(LDFLAGS) $(C_OBJS) -o client.out 

server.o: socket.h
client.o: socket.h
socket.o: socket.h

.PHONY: clean
clean:
	rm -f *.o *.out *.gch
