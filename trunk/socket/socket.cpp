/*
 * File: socket.cpp
 * Date: May, 18/2012
 * Description: Contains implementations of Socket and ServerSocket classes
 *
 * TODO:
 * [ ] Improve sendData and receiveData functions
 * [ ] Improve error handling
 */

#include "socket.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* ---------- Socket ---------- */

/* 
 * Socket constructor. Calls socket() to get a socket file descriptor.
 */
Socket::Socket() : server(0) {
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
Socket::Socket(int fd) : sockfd(fd), server(0) {
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
Socket::Socket(std::string hostname, int portNumber) : server(0) {
	// Create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
		perror("Could not open socket");
		exit(1);
	}
	
	connectTo(hostname, portNumber);
}

/*
 * Socket desctructor. Closes the file descriptor held by this object
 */
Socket::~Socket() {
	close(sockfd);
	delete server;
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
 * Sends the given data via connection-oriented socket
 * msg: Message to be sent
 * length: Length of the message in number of bytes
 */
int Socket::sendData(std::string msg, int length) {
	int n = 0;

	n = write(sockfd, (char *) msg.c_str(), length);

	if (n < 0) {
		perror("Failed to write to socket");
		exit(1);
	}

	return n;
}

/*
 * Receives data from a connection-oriented socket
 */
char* Socket::receiveData() {
	int n = 0;

	n = read(sockfd, buffer, 255);
	if (n < 0) {
		perror("Unable to read from socket");
		exit(1);
	}

	return buffer;
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
 * Accepts a connection request from a client. Uses the blocking accept() call
 */
Socket ServerSocket::acceptConnection() {
	socklen_t clientLength = sizeof clientAddr;
	int newSockfd = accept(sockfd, 
			(struct sockaddr *) &clientAddr,
			&clientLength);

	return Socket(newSockfd);
}
