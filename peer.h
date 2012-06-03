/*
 * File peer.h
 * Group: 14
 * Description: Header file for the Peer class which represents the remote peers
 * in the network.
 */

#ifndef _PEER_H_
#define _PEER_H_

#include <string>

// Forward declarations
class Status;

class Peer {
	public:
		Peer();

		void setIpAddress(std::string const ipAddress) { _ipAddress = ipAddress; }
		void setPortNumber(int const portNumber) { _portNumber = portNumber; }
	private:
		std::string _ipAddress;
		int _portNumber;
		enum State { connected, disconnected, unknown } _state;
};

#endif  /* _PEER_H_ */
