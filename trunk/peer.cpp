/*
 * File.peer.cpp
 * Group: 14
 * Description: Implementation for the Peer class.
 */

#include <sstream>
#include "peer.h"
#include "constants.h"
#include "util.h"
#include <iostream>	// remove

Peer::Peer() : 
_ipAddress(""), _portNumber(0), _state(Peer::disconnected), _threadId(0) {
	std::cout << "Peer constructor" << std::endl;
	// Empty
}

/*
 * Destructor
 */
Peer::~Peer() {
	std::cout << "Peer destructor" << std::endl;
	stopThread();
	_socket.closeConnection();
}

/*
 * Attempts to connect to the peer. Will not be able to receive any data until
 * a new thread is started with a subsquent start() call.
 */
bool Peer::connect() {
	if (_socket.connectTo(_ipAddress, _portNumber)) {
		_state = connected;
		return true;
	} else {
		_state = disconnected;
		return false;
	}
}

/*
 * Close socket connection to peer.
 */
void Peer::disconnect() {
	Log::info("in Peer::disconnect()");
	stopThread();
	_socket.closeConnection();
	_state = disconnected;
}

/*
 * Sends the given Message object to the peer
 *
 * msg: The Message object to be sent
 */
void Peer::sendMessage(Message& msg) {
	std::stringstream ss;
	std::string serializedMsg;

	ss << msg;
	serializedMsg = ss.str();

	_socket.sendData(serializedMsg, serializedMsg.length());
}

/*
 * Inherited from Thread class. Runs in a new thread. Responsible for 
 * receiving data from the socket and placing it in a synchronized receive 
 * queue.
 */
void Peer::run() {
	Log::info("in Peer::run()");
	while (true) {
		if (_state != connected)
			continue;

		Log::info("trying to receive data from peer");

		// Retrieve the size of the data frame to follow
		char sizeBuffer[constants::SIZE_BUFFER_SIZE];
		_socket.receiveData(sizeBuffer, constants::SIZE_BUFFER_SIZE);
		
		// Now retreive actual data frame
		int size = ntohl(*(int *)sizeBuffer);
		char dataBuffer[size];
		_socket.receiveData(dataBuffer, size);

		// Deserialize data into Message object and put in receive queue
		Message msg;
		std::stringstream ss(dataBuffer);
		ss >> msg;
		_receiveQueue.push(msg);
	}
}
