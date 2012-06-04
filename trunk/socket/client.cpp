// Test code for socket IO with threads

#include "socket.h"
#include <iostream>

int main() {
	std::cout << "Hello from client" << std::endl;

	Socket clientSocket("127.0.0.1", 8081);
	std::cout << "ClientSocket created" << std::endl;

	std::string msg = "hello, this is a long sentence";
	clientSocket.sendData(msg, msg.length());
	std::cout << "after sendData()" << std::endl;

	return 0;
}
