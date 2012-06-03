# Makefile
# May, 18/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=-pthread

S_OBJS=socket.o server.o
C_OBJS=socket.o client.o

all: server.out client.out

# Server executable
server.out: $(S_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o server.out $(S_OBJS)

# Client executable
client.out: $(C_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o client.out $(C_OBJS)

server.o: socket.h
client.o: socket.h
socket.o: socket.h

.PHONY: clean
clean:
	rm -f *.o *.out *.gch
