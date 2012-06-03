# Makefile
# May, 19/2012

CXX=g++
CFLAGS=-Wall -g
LD=g++
LDFLAGS=
LIBS=-lpthread

OBJS= test.o

all: a.out

a.out: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LIBS) $(OBJS)

test.o: concurrent_queue.h

.PHONY: clean
clean:
	rm -f *.o *.out *.gch
