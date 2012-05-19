/*
 * File: socket.cpp
 * Date: May, 18/2012
 * Description: Contains implementation of Socket, ServerSocket and
 * ClientSocket classes used to communication
 *
 * TODO:
 * [ ] Modify to accept binary data instead of strings
 * [ ] Make it threaded
 */

#include "socket.h"

using namespace std;

void error(string msg) {
	cerr << "ERROR: " << msg << endl;
	exit(1);
}

/* ---------- Socket ---------- */
/*
 * Socket constructor. Calls socket() to get a socket file descriptor.
 */
Socket::Socket() {
	cout << "Creating Socket" << endl;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("Could not open socket");
}

/*
 * Socket constructor. Creates a new socket based on an exiting socket file descriptor
 * fd: File descriptor of an existing socket
 */
Socket::Socket(int fd): sockfd(fd) {
	if (sockfd < 0) 
		error("Invalid socket file descriptor");
}

/*
 * Socket desctructor. Closes the file descriptor held by this object
 */
Socket::~Socket() {
	close(sockfd);
}

/*
 * Sends the given data via connection-oriented socket
 * msg: Message to be sent
 * length: Length of the message in number of bytes
 */
int Socket::sendData(string msg, int length) {
	int n = 0;

	n = write(sockfd, (char *) msg.c_str(), length);

	if (n < 0)
		error("Failed to write to socket");

	return n;
}

/*
 * Receives data from a connection-oriented socket
 */
char* Socket::receiveData() {
	int n = 0;

	n = read(sockfd, buffer, 255);
	return buffer;
}

/* ---------- ServerSocket ---------- */
/*
 * ServerSocket constructor. Creates a socket, binds it to the given
 * portNumber and calls listen() to mark it as a passive socket
 */
ServerSocket::ServerSocket(unsigned short portNumber) {
	cout << "Creating ServerSocket" << endl;

	// Clear then populate serverAddr
	memset(&serverAddr, 0, sizeof serverAddr);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(portNumber);

	// Bind
	if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof serverAddr) < 0)
		error("Unable to bind socket");

	// Listen
	listen(sockfd, 5);
}

/*
 * ServerSocket desctructor.
 */
ServerSocket::~ServerSocket() {
	delete connectedSocket;
}

/*
 * Accepts a connection request from a client. Uses the blocking accept() call
 */
void ServerSocket::acceptConnection() {
	socklen_t clientLength = sizeof clientAddr;
	int newSockfd = accept(sockfd, 
			(struct sockaddr *) &clientAddr,
			&clientLength);

	connectedSocket = new Socket(newSockfd);
	cout << "Accepted connection from client" << endl;
}

/*
 * Sends data using the connected client socket
 * msg: Message to be sent
 * length: Length of the message in number of bytes
 */
int ServerSocket::sendData(string msg, int length) {
	return connectedSocket->sendData(msg, length);
}

/*
 * Receives data using the connected client socket
 */
char* ServerSocket::receiveData() {
	return connectedSocket->receiveData();
}

/* ---------- ClientSocket ---------- */
/*
 * ClientSocket constructor. Creates a socket then connects to a server using the created socket.
 * hostname: Name of the server
 * portNumber: Port number of the server
 */
ClientSocket::ClientSocket(string hostname, unsigned short portNumber) {
	cout << "Creating ClientSocket" << endl;

	server = gethostbyname(hostname.c_str());

	if (server == NULL) 
		error("Unable to get host by name");

	// Clear then populate serverAddr
	memset(&serverAddr, 0, sizeof serverAddr);
	serverAddr.sin_family = AF_INET;
	memcpy(server->h_addr, &serverAddr.sin_addr.s_addr, server->h_length);
	serverAddr.sin_port = htons(portNumber);

	// Connect to server
	if (connect(sockfd, (struct sockaddr *) &serverAddr, sizeof serverAddr) , 0)
		error("Unable to connect");

	cout << "Connected to server" << endl;	
}
