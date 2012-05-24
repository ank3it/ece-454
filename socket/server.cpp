// Test code for socket IO with threads

#include "socket.h"
#include <pthread.h>

void* receiveStuff(void* params) {
	Socket* socket = (Socket *)params;

	std::string receivedMsg;
	while (receivedMsg.compare("exit") != 0) {
		receivedMsg = socket->receiveData();
		std::cout << "Client: " << receivedMsg << std::endl;
	}

	return NULL;
}

int main() {
	std::cout << "Hello from server" << std::endl;

	pthread_t thread_id1;

	ServerSocket serverSocket(8080);
	std::cout << "ServerSocket created" << std::endl;

	Socket clientSocket = serverSocket.acceptConnection();

	pthread_create(&thread_id1, NULL, &receiveStuff, &clientSocket);

	std::string msg;
	while (msg.compare("exit") != 0) {
		getline(std::cin, msg);
		clientSocket.sendData(msg, msg.length());
	}

	pthread_cancel(thread_id1);
	pthread_join(thread_id1, NULL);

	return 0;
}
