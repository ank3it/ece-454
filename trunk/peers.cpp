/*
 * File: peers.cpp
 * Date: Jun. 1/2012
 * Group: 14
 * Description: Implementation for the peers class. This class is a container
 * for all the peers that are in a network.
 */

#include <fstream>
#include <cstdlib>
#include "return_codes.h"
#include "peers.h"

Peers::Peers() : _numPeers(0) {
	// Empty constructor
}

/*
 * Reads the peersFile and populates a Peer object with the IP address and port
 * number for each entry in the file
 *
 * peersFile: The name of peers file containing a list of the ip addresses and
 * 			  port numbers of the peers
 */
int Peers::initialize(std::string peersFile) {
	std::ifstream file;
	std::string lineItem;
	std::string ipAddress;
	int portNumber;

	file.open(peersFile.c_str());

	if (!file.is_open())
		return returnCodes::ERROR_NO_PEERS_FOUND;

	while (file >> lineItem) {
		// Stop after maxPeers and return with warning
		if (_numPeers >= MAX_PEERS)
			return returnCodes::WARNING_TOO_MANY_PEERS;

		ipAddress = lineItem;

		if (file >> lineItem)
			portNumber = atoi(lineItem.c_str());
		else
			return -1;

		_peers[_numPeers].setIpAddress(ipAddress);
		_peers[_numPeers].setPortNumber(portNumber);
		_numPeers++;
	}

	// Peers file did not contain any peers
	if (_numPeers == 0)
		return returnCodes::ERROR_NO_PEERS_FOUND;

	return returnCodes::OK;
}

Peer& Peers::operator ()(int i) {
	return _peers[i];
}
