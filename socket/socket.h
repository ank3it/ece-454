/*
 * File: socket.h
 * Date: May, 18/2012
 * Group: 14
 * Description: Header file for Socket and ServerSocket.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* Constants */
const int BUFFER_SIZE = 256;

/*
 * Represents a single socket. Stores the necessary socket file descriptor and 
 * provides function to send and receive data via the socket
 */
class Socket {
	public:
		Socket();
		Socket(int);
		Socket(std::string, int);
		virtual ~Socket();

		int sendData(std::string, int);
		char* receiveData();

	protected:
		int sockfd;
		int portNumber;
		char buffer[BUFFER_SIZE];
		struct sockaddr_in serverAddress;
	
	private:
		struct hostent* server;
};

/*
 * Socket which allows multiple clients to connect. Creates new Socket for each
 * client and spawns a new thread
 */
class ServerSocket : public Socket {
	public:
		ServerSocket(unsigned short);
		~ServerSocket();

		Socket acceptConnection();

	private:
		struct sockaddr_in clientAddr;
};

#endif  /* _SOCKET_H_ */
