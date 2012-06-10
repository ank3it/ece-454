// Test code for socket IO with threads

#include "socket.h"
#include <iostream>
#include <netinet/in.h>

int main() {
	std::cout << "Hello from server" << std::endl;

	ServerSocket serverSocket(8080);
	std::cout << "ServerSocket created" << std::endl;

	Socket clientSocket = serverSocket.acceptConnection();
	std::cout << "Client connection accepted" << std::endl;

	char buf[4];
	clientSocket.receiveData(buf, 4);
	int size = ntohl(*(int*)buf);
	std::cout << "size = " << size << std::endl;

	char buf2[size];
	int rbc = clientSocket.receiveData(buf2, size);
	std::cout << "buf2 = " << buf2 << std::endl;
	std::cout << "rbc = " << rbc << std::endl;

	return 0;
}
