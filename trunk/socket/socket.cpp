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
#include "../util.h"
#include <iostream> // remove

/* ---------- Socket ---------- */

/* 
 * Socket constructor. Calls socket() to get a socket file descriptor.
 */
Socket::Socket() {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		Log::error("Could not open socket");
	}
}

/*
 * Socket constructor. Creates a new socket based on an exiting socket file 
 * descriptor
 * fd: File descriptor of an existing socket
 */
Socket::Socket(int fd) : sockfd(fd) {
	if (sockfd < 0) {
		Log::error("Invalid socket file descriptor");
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
		Log::error("Could not open socket");
	}
	
	connectTo(hostname, portNumber);
}

bool Socket::connectTo(std::string hostname, int portNumber) {
	// Get server information
	server = gethostbyname(hostname.c_str());
	if (server == NULL) {
		Log::error("Unable to get host by name");
		return false;
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
		Log::warn("Unable to connect");
		return false;
	}

	return true;
}

/*
 * Close the socket
 */
void Socket::closeConnection() {
	Log::info("Socket::closeConnection()");
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
	//Log::info("in Socket::sendData()");
	int n = 0;

	int nextFrameSize = htonl(length);
	if (send(sockfd, &nextFrameSize, sizeof(nextFrameSize), 0) < 0) {
		Log::error("Failed to send size header");
	}

	n = send(sockfd, (char *)msg.c_str(), length, 0);
	if (n < 0) {
		Log::error("Failed to write to socket");
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
int Socket::receiveData(char* buffer, int size) {
	//Log::info("in receiveData()");
	int receivedByteCount = 0;

	receivedByteCount = read(sockfd, buffer, size);

	if (receivedByteCount > 0 && receivedByteCount < size) {
		Log::info("calling receiveData() again");
		receiveData(buffer + receivedByteCount, size - receivedByteCount);
	} else if (receivedByteCount == 0) {
		Log::warn("Couldn't receive, other socket has closed");
	} else if (receivedByteCount < 0) {
		Log::error("receiveData() couldn't receive any data");
	}
	
	return receivedByteCount;
}

/* ---------- ServerSocket ---------- */

/*
 * ServerSocket constructor. Creates a socket, binds it to the given
 * portNumber and calls listen() to mark it as a passive socket
 */
ServerSocket::ServerSocket(unsigned short portNumber) {
	Log::info("in ServerSocket constructor()");
	// Clear then populate serverAddress
	memset(&serverAddress, 0, sizeof serverAddress);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);

	// Bind
	if (bind(sockfd, (struct sockaddr *) &serverAddress, 
		sizeof serverAddress) < 0) {
		Log::error("Unable to bind socket");
	}
	Log::info("ServerSocket bind() successful");

	// Listen
	listen(sockfd, 5);
}

/*
 * Accepts a connection request from a client. Uses the blocking accept() call.
 * Returns the socket which can used to communicate with the client.
 */
Socket ServerSocket::acceptConnection() {
	Log::info("in ServerSocket::acceptConnection()");
	socklen_t clientLength = sizeof clientAddr;
	int newSockfd = accept(sockfd, 
			(struct sockaddr *) &clientAddr,
			&clientLength);

	return Socket(newSockfd);
}
