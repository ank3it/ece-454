// Test code for socket IO with threads

#include "socket.h"
#include <iostream>

int main() {
	std::cout << "Hello from client" << std::endl;

	Socket clientSocket("127.0.0.1", 8080);
	std::cout << "ClientSocket created" << std::endl;

	char buf[130714];
	buf[0] = 'h';
	buf[1] = 'e';
	buf[2] = 'l';
	buf[3] = 'l';
	buf[4] = 'o';
	clientSocket.sendData(buf, 130714);
	std::cout << "after sendData()" << std::endl;

	return 0;
}
