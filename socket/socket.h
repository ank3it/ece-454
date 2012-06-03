/*
 * File: socket.h
 * Date: May, 18/2012
 * Group: 14
 * Description: Header file for Socket and ServerSocket.
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <netdb.h>
#include <string>

/* Constants */
const int BUFFER_SIZE = 256;

/*
 * Represents a single socket. Stores the necessary socket file descriptor and 
 * provides functions to send and receive data via the socket.
 */
class Socket {
	public:
		Socket();
		Socket(int);
		Socket(std::string, int);
		~Socket();

		bool connectTo(std::string, int);
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
 * Socket which allows clients to connect to it.
 */
class ServerSocket : public Socket {
	public:
		ServerSocket(unsigned short);

		Socket acceptConnection();

	private:
		struct sockaddr_in clientAddr;
};

#endif  /* _SOCKET_H_ */
