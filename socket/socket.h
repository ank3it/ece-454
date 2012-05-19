/*
 * File: socket.h
 * Date: May, 18/2012
 * Group: 14
 * Description: Header file for Socket, ServerSocket and ClientSocket
 * classes.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

class Socket {
	public:
		Socket();
		Socket(int);
		virtual ~Socket();

		int sendData(string, int);
		char* receiveData();

	protected:
		int sockfd;
		int portNumber;
		char buffer[256];
};

class ServerSocket : public Socket {
	public:
		ServerSocket(unsigned short);
		virtual ~ServerSocket();

		void acceptConnection();
		int sendData(string, int);
		char* receiveData();

	private:
		struct sockaddr_in serverAddr;
		struct sockaddr_in clientAddr;
		Socket* connectedSocket;
};

class ClientSocket : public Socket {
	public:
		ClientSocket(string, unsigned short);
	private:
		struct sockaddr_in serverAddr;
		struct hostent* server;
};

#endif /* _SOCKET_H_ */
