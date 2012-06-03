/*
 * File.peer.cpp
 * Group: 14
 * Description: Implementation for the Peer class.
 */

#include "peer.h"
#include <iostream>	// remove

Peer::Peer() : 
_ipAddress(""), _portNumber(0), _state(Peer::disconnected), _threadId(0) {
	std::cout << "Peer constructor" << std::endl;
	// Empty
}

// remove
Peer::~Peer() {
	std::cout << "Peer destructor" << std::endl;
}

/*
 * Attempts to open a connection-based socket to the peer. Also spawns a new 
 * thread to process send and receive requests.
 */
bool Peer::connect() {
	if (socket.connectTo(_ipAddress, _portNumber)) {
		start();
		return true;
	} else {
		return false;
	}
}

/*
 * Inherited from Thread class. Performs socket based I/O operations
 */
void Peer::run() {
	std::cout << "hello world" << std::endl;
}
