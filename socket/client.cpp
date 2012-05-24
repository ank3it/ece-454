// Test code for socket IO with threads

#include "socket.h"
#include <pthread.h>

void* receiveStuff(void* params) {
	Socket* socket = (Socket *)params;

	// Receive message from server and print to console
	std::string receivedMsg;
	while (receivedMsg.compare("exit") != 0) {
		receivedMsg = socket->receiveData();
		std::cout << "Server: " << receivedMsg << std::endl;
	}

	return NULL;
}

int main() {
	std::cout << "Hello from client" << std::endl;

	pthread_t thread_id1;

	Socket clientSocket("127.0.0.1", 8080);

	std::cout << "ClientSocket created" << std::endl;

	// Create new thread for receiving messages from server
	pthread_create(&thread_id1, NULL, &receiveStuff, &clientSocket);

	// Get input and send to server
	std::string msg;
	while (msg.compare("exit") != 0) {
		getline(std::cin, msg);
		clientSocket.sendData(msg, msg.length());
	}

	pthread_cancel(thread_id1);
	pthread_join(thread_id1, NULL);

	return 0;
}
