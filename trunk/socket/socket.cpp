/*
 * File: socket.cpp
 * Date: May, 18/2012
 * Description: Contains implementations of Socket and ServerSocket classes
 */

#include "socket.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream> // remove

/* ---------- Socket ---------- */

/* 
 * Socket constructor. Calls socket() to get a socket file descriptor.
 */
Socket::Socket() {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("Could not open socket");
		exit(1);
	}
}

/*
 * Socket constructor. Creates a new socket based on an exiting socket file 
 * descriptor
 * fd: File descriptor of an existing socket
 */
Socket::Socket(int fd) : sockfd(fd) {
	if (sockfd < 0) {
		perror("Invalid socket file descriptor");
		exit(1);
	}
}

/*
 * Socket constructor. Creates a socket then connects to a server using 
 * the created socket.
 * hostname: Name of the server
 * portNumber: Port number of the server
 */
Socket::Socket(std::string hostname, int portNumber) {
	// Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
		perror("Could not open socket");
		exit(1);
	}
	
	connectTo(hostname, portNumber);
}

bool Socket::connectTo(std::string hostname, int portNumber) {
	// Get server information
	server = gethostbyname(hostname.c_str());
	if (server == NULL) {
		perror("Unable to get host by name");
		exit(1);
	}

	// Clear then populate serverAddress
	memset(&serverAddress, 0, sizeof serverAddress);
	serverAddress.sin_family = AF_INET;
	memcpy(server->h_addr, &serverAddress.sin_addr.s_addr, server->h_length);
	serverAddress.sin_port = htons(portNumber);

	// Connect to server
	if (connect(sockfd, 
				(struct sockaddr *) &serverAddress, 
				sizeof serverAddress) < 0) {
		perror("Unable to connect");
		exit(1);
	}

	return true;
}

/*
 * Close the socket
 */
void Socket::closeConnection() {
	close(sockfd);
}

/*
 * Sends the given data via connection-oriented socket. First sends the size of
 * msg and then sends the msg itself. Returns the number of bytes sent.
 *
 * msg: Message to be sent
 * length: Length of the message in number of bytes
 */
int Socket::sendData(std::string msg, int length) {
	int n = 0;

	int nextFrameSize = htonl(length);
	if (send(sockfd, &nextFrameSize, sizeof(nextFrameSize), 0) < 0) {
		perror("Failed to send size header");
		exit(1);
	}

	n = send(sockfd, (char *)msg.c_str(), length, 0);
	if (n < 0) {
		perror("Failed to write to socket");
		exit(1);
	}

	return n;
}

/*
 * Receives data from a connection-oriented socket. Returns the number of bytes
 * received. Will make multiple attempts to read data until size bytes have
 * been received.
 *
 * buffer: The buffer in which the result will be returned
 * size: The size of the data to read
 */
void Socket::receiveData(char* buffer, int size) {
	std::cout << "receiveData()" << std::endl << sockfd << std::endl;
	int receivedByteCount = 0;

	receivedByteCount = recv(sockfd, buffer, size, 0);

	if (receivedByteCount < 0) {
		perror("Unable to read from socket");
		exit(1);
	} else if (receivedByteCount < size) {
		receiveData(buffer + receivedByteCount, size - receivedByteCount);
	}
}

/* ---------- ServerSocket ---------- */

/*
 * ServerSocket constructor. Creates a socket, binds it to the given
 * portNumber and calls listen() to mark it as a passive socket
 */
ServerSocket::ServerSocket(unsigned short portNumber) {
	// Clear then populate serverAddress
	memset(&serverAddress, 0, sizeof serverAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);

	// Bind
	if (bind(sockfd, 
			(struct sockaddr *) &serverAddress, 
			sizeof serverAddress) < 0) {
		perror("Unable to bind socket");
		exit(1);
	}

	// Listen
	listen(sockfd, 5);
}

/*
 * Accepts a connection request from a client. Uses the blocking accept() call.
 * Returns the socket which can used to communicate with the client.
 */
Socket ServerSocket::acceptConnection(std::string& ipAddress, int& portNumber) {
	socklen_t clientLength = sizeof clientAddr;
	int newSockfd = accept(sockfd, 
			(struct sockaddr *) &clientAddr,
			&clientLength);

	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientAddr.sin_addr), str, INET_ADDRSTRLEN);
	ipAddress = str;
	portNumber = ntohs(clientAddr.sin_port);

	return Socket(newSockfd);
}
