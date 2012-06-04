#include <iostream>
#include "socket/socket.h"

int main() {
	std::cout << "test.cpp" << std::endl;

	Socket s("127.0.0.1", 8080);
	std::cout << "client connected" << std::endl;

	while (true);
}
