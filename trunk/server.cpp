#include "server.h"
#include "constants.h"

Server::Server() : _serverSocket(0), _peers(0) {
	// Empty
}

Server::Server(ServerSocket* ss, Peers* p) : _serverSocket(ss), _peers(p) {
	// Empty
}

/*
 * Destructor. Stop the thread before destructing
 */
Server::~Server() {
	stopServer();
}

/*
 * Startst the server thread, enabling it to accept connections from other peers.
 * Returns true if thread was start successfully.
 */
bool Server::startServer() {
	return startThread();
}

/*
 * Stops the server thread.
 */
bool Server::stopServer() {
	return stopThread();
}

/*
 * Inherited from the Thread class. Runs in another thread. Listens for other
 * peers trying to connect.
 */
void Server::run() {
	while (true) {
		// Don't accept connections if already at max number of peers
		if (_peers->getNumPeers() >= constants::MAX_PEERS)
			continue;

		Socket clientSocket = _serverSocket->acceptConnection();

		Peer* p = new Peer();
		p->setSocket(clientSocket);
		p->setState(Peer::connected);

		// Start Peer thread so we can begin receiving data from the peer
		if (!p->startThread()) {
			// Error could not start Peer thread
		}
	}
}
