/*
 * File	local_peer.cpp
 * Group: 14
 * Description: Implementation for the LocalPeer class.
 *
 * TODO:
 * [ ] Create filesDir if it doesn't exist
 * [ ] Add better error handling
 * [ ] Add logic to push file to peers in insert()
 */

#include <fstream>
#include <cstdlib>
#include "local_peer.h"
#include "return_codes.h"
#include "util.h"
#include <iostream> // remove

/*
 * Constructor. Creates a new Peers object
 */
LocalPeer::LocalPeer() {
	std::cout << "LocalPeer constructor" << std::endl;
	// Empty constructor
}

// remove
LocalPeer::~LocalPeer() {
	std::cout << "LocalPeer destructor" << std::endl;
}

/*
 * Adds the given file to the set of files that are/will be replicated across
 * the network. A copy of the file is placed with the other files that are being
 * synced and then uploaded to other peers via the BitTorrent protocol.
 *
 * filepath: The file to be replicated
 */
int LocalPeer::insert(std::string filepath) {
	char* buffer;
	std::ifstream inFile(filepath.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	// Copy file
	if (inFile.is_open()) {
		// Read file into buffer
		std::ifstream::pos_type size = inFile.tellg();
		buffer = new char[size];

		inFile.seekg(0, std::ios::beg);
		inFile.read(buffer, size);

		// Write file from buffer
		std::string outFilename = Util::generateUniqueFilename(
			filesDir, Util::extractFilename(filepath));

		std::ofstream outFile(outFilename.c_str(), 
			std::ios::out | std::ios::binary);

		if (outFile.is_open()) {
			outFile.write(buffer, size);
		} else {
			return -1;
		}

		outFile.close();
		inFile.close();
	} else {
		return -1;
	}

	delete[] buffer;
	return returnCodes::OK;
}

int LocalPeer::query(Status& status) {
	return 0;
}

/*
 * Attempts to join the set of peers. Pushes any local files out to the peers
 * and also receives any files that the peers might have that is not present
 * locally.
 */
int LocalPeer::join() {
	int peersConnected = 0;
	_peers.initialize(peersList);

	for (int i = 0; i < _peers.getNumPeers(); i++) {
		if (_peers(i).connect())
			peersConnected++;
	}

	if (peersConnected < _peers.getNumPeers()) {
		return returnCodes::WARNING_PEER_NOT_FOUND;
	}

	return returnCodes::OK;
}

int LocalPeer::leave() {
	return 0;
}
