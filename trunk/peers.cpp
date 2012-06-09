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
#include "constants.h"
#include "peers.h"
#include "util.h"
#include <iostream> // remove

Peers::Peers() {
	// Empty
}

/*
 * Destructor. Iterates through the peers list and destroys all Peer objets.
 */
Peers::~Peers() {
	std::cout << "Peers destructor()" << std::endl;
	std::list<Peer*>::iterator it;

	for (it = _peers.begin(); it != _peers.end(); ++it)
		delete *it;
}

/*
 * Reads the peersFile and populates a Peer object with the IP address and port
 * number for each entry in the file
 *
 * peersFile: The name of peers file containing a list of the ip addresses and
 * 			  port numbers of the peers
 */
int Peers::initialize(std::string peersFile) {
	Log::info("in Peers::initialize()");
	std::ifstream file;
	std::string lineItem;
	std::string ipAddress;
	int portNumber;
	int numPeers = 0;

	file.open(peersFile.c_str());

	if (!file.is_open())
		return returnCodes::ERROR_NO_PEERS_FOUND;

	while (file >> lineItem) {
		// Stop after maxPeers and return with warning
		if (_peers.size() >= constants::MAX_PEERS)
			return returnCodes::WARNING_TOO_MANY_PEERS;

		ipAddress = lineItem;

		if (file >> lineItem)
			portNumber = atoi(lineItem.c_str());
		else
			return -1;

		Peer* p = new Peer();
		p->setIpAddress(ipAddress);
		p->setPortNumber(portNumber);
		p->setState(Peer::disconnected);
		_peers.push_back(p);
		++numPeers;
	}

	// Peers file did not contain any peers
	if (numPeers == 0)
		return returnCodes::ERROR_NO_PEERS_FOUND;

	return returnCodes::OK;
}

/*
 * Attempts to connect to all known peers by iterating through the list of 
 * peers. Starts the threads associated with each peer as well.
 */
int Peers::connectToAllPeers() {
	int peersConnected = 0;
	std::list<Peer*>::iterator it = _peers.begin();

	while (it != _peers.end()) {
		if ((*it)->connect()) {
			peersConnected++;

			// Start Peer thread so we can begin receiving data from the peer
			if (!(*it)->startThread())
				return returnCodes::ERROR_UNKNOWN;

			++it;
		} else {
			// Unable to connect, remove Peer from the list of peers
			_peers.erase(it++);
		}
	}

	return returnCodes::OK;
}

/*
 * Returns a reference to the Peer object specified by the given index
 */
Peer& Peers::operator()(int i) {
	std::list<Peer*>::iterator it;
	it = _peers.begin();

	for (int j = 0; j < i; ++j)
		++it;

	return **it;
}
