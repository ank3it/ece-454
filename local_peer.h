/*
 * File local_peer.h
 * Group: 14
 * Description: Header file for the LocalPeer class which represents the machine
 * on which this code is running.
 */

#ifndef _LOCAL_PEER_H_
#define _LOCAL_PEER_H_

#include <string>

const int chunkSize = 65536;

class Status;
class Peers;

class LocalPeer {
	public:
		LocalPeer();
		~LocalPeer();

		// Required interface
		int insert(std::string filename);
		int query(Status& status);
		int join();
		int leave();
		// End of required interface

		void setIpAddress(std::string const ipAddress) { _ipAddress = ipAddress; }
		void setPortNumber(int const portNumber) { _portNumber = portNumber; }
	private:
		std::string _ipAddress;
		int _portNumber;
		enum State { connected, disconnected, unknown } _state;
		Peers* _peers;
};

#endif  /* _LOCAL_PEER_H_ */
