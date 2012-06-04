/*
 * File	local_peer.cpp
 * Group: 14
 * Description: Implementation for the LocalPeer class.
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
	_serverSocket = new ServerSocket(PORT_NUMBER);
}

/*
 * Destructor
 */
LocalPeer::~LocalPeer() {
	std::cout << "LocalPeer destructor" << std::endl;
	_serverSocket->closeConnection();
	delete _serverSocket;
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

	if (!inFile.is_open())
		return returnCodes::ERROR_FILE_NOT_FOUND;

	// Copy file
	// Read file into buffer
	std::ifstream::pos_type size = inFile.tellg();
	buffer = new char[size];

	inFile.seekg(0, std::ios::beg);
	inFile.read(buffer, size);

	// Write file from buffer
	std::string outFilename = Util::generateUniqueFilename(
		FILES_DIR, Util::extractFilename(filepath));

	std::ofstream outFile(outFilename.c_str(), 
		std::ios::out | std::ios::binary);

	if (!outFile.is_open())
		return returnCodes::ERROR_UNKNOWN;

	outFile.write(buffer, size);

	outFile.close();
	inFile.close();

	delete[] buffer;

	return returnCodes::OK;
}

int LocalPeer::query(Status& status) {
	return 0;
}

/*
 * Attempts to join the set of peers by iterating throught the list of peers. 
 * Pushes any local files out and also receives any files that the peers might
 * have that are not present locally.
 */
int LocalPeer::join() {
	int peersConnected = 0;
	_peers.initialize(PEERS_LIST);

	// Connect to each Peer in Peers container
	for (int i = 0; i < _peers.getNumPeers(); i++) {
		if (_peers(i).connect()) {
			peersConnected++;

			// Start Peer thread so we can begin receiving data from the peer
			if (!_peers(i).start())
				return returnCodes::ERROR_UNKNOWN;
		}
	}

	if (peersConnected == 0)
		return returnCodes::ERROR_CANNOT_CONNECT;

	// Start local peer thread to begin accepting connections
	if (!start())
		return returnCodes::ERROR_UNKNOWN;

	if (peersConnected < _peers.getNumPeers())
		return returnCodes::WARNING_PEER_NOT_FOUND;

	return returnCodes::OK;
}

int LocalPeer::leave() {
	return 0;
}

/*
 * Implemented from Thread class. Runs in another thread. Listens for other
 * peers trying to connect to this peer.
 */
void LocalPeer::run() {
	std::string ipAddress;
	int portNumber;

	while (true) {
		// Don't accept connections if already at max number of peers
		if (_peers.getNumPeers() >= MAX_PEERS)
			continue;

		Socket clientSocket = 
			_serverSocket->acceptConnection(ipAddress, portNumber);

		int i = _peers.getNumPeers();
		_peers(i).setSocket(clientSocket);
		_peers(i).setState(Peer::connected);

		// Start Peer thread so we can begin receiving data from the peer
		if (!_peers(i).start()) {
			// Error could not start Peer thread
		}

		_peers.setNumPeers(++i);
	}
}
