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
#include "constants.h"
#include "queue/concurrent_queue.h"
#include "message.h"
#include <sstream>
#include <cmath>
#include <iostream> // remove

/*
 * Constructor. Creates new ServerSocket and Server objects using the default
 * port number.
 */
LocalPeer::LocalPeer() {
	std::cout << "LocalPeer constructor()" << std::endl;
	_serverSocket = new ServerSocket(constants::DEFAULT_PORT_NUMBER);
	_server = new Server(_serverSocket, &_peers);
}

/*
 * Constructor. Create new ServerSocket and Server objects using the given 
 * port number.
 *
 * portNum: The portNumber on which this local peer will listen for connections.
 */
LocalPeer::LocalPeer(unsigned short portNum) : _portNumber(portNum) {
	std::cout << "LocalPeer constructor(portNum)" << std::endl;
	_serverSocket = new ServerSocket(_portNumber);
	_server = new Server(_serverSocket, &_peers);
}

/*
 * Destructor
 */
LocalPeer::~LocalPeer() {
	std::cout << "LocalPeer destructor()" << std::endl;
	stopThread();
	_serverSocket->closeConnection();
	delete _server;
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
	_fileManager.addLocalFile(filepath);
	broadcastFileNotification(Util::extractFilename(filepath));
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
	_peers.initialize(constants::PEERS_LIST);
	int returnCode = _peers.connectToAllPeers();

	// Start server so it can begin accepting connections
	if (!_server->startServer())
		return returnCodes::ERROR_UNKNOWN;

	// Broadcast files to peers
	std::map<std::string, File*>* ft = _fileManager.getFilesTable();
	std::map<std::string, File*>::iterator it;

	for (it = ft->begin(); it != ft->end(); ++it){
		broadcastFileNotification(it->first);
	}

	return returnCode;
}

/*
 * Leaves the set of peers. The local peer informs all other peers that it is
 * leaving. Attempts to push any remaining least replicated chunks out to the 
 * peers before leaving if there is a small number of them.
 */
int LocalPeer::leave() {
	broadcastLeaveNotification();

	for (int i = 0; i < _peers.getNumPeers(); i++) {
		_peers(i).disconnect();
	}

	return returnCodes::OK;
}

/*
 * Notifies the other peers about the given file
 *
 * filename: The name of the file
 */
void LocalPeer::broadcastFileNotification(std::string filename) {
	// Construct message
	std::stringstream ss;
	ss << *(_fileManager.getFile(filename));
	Message m(Message::FILE_NOTIFICATION, ss.str());

	// Iterate through all peers, sending them the message
	std::list<Peer*>* peersList = _peers.getPeersList();
	std::list<Peer*>::iterator it;

	for (it = peersList->begin(); it != peersList->end(); ++it)
		(*it)->sendMessage(m);
}

/*
 * Notifies the other peers that the peer is leaving the network.
 */
void LocalPeer::broadcastLeaveNotification() {
	Message m(Message::LEAVE_NOTIFICATION, "");
	std::list<Peer*>* peersList = _peers.getPeersList();
	std::list<Peer*>::iterator it;

	for (it = peersList->begin(); it != peersList->end(); ++it)
		(*it)->sendMessage(m);
}

/*
 * Inherited from Thread class. Runs in another thread. Process messages
 * received from other peers by checking each peer's receive message queue. Also
 * sends out file chunk requests to other peers.
 */
void LocalPeer::run() {
	// Process messages from peers
	std::list<Peer*>* peersList = _peers.getPeersList();
	while (true) {
		std::list<Peer*>::iterator it;

		for (it = peersList->begin(); it != peersList->end(); ++it) {
			ConcurrentQueue<Message>* q = (*it)->getReceiveQueue();
			Message m;

			// Check peer's receive queue
			if (q->tryPop(m)) {
				switch(m.getMessageType()) {
					case Message::LEAVE_NOTIFICATION:
						// Remove peer from list of peers
						peersList->erase(it++);
						continue;
						break;
					case Message::FILE_CHUNK:
					{
						// Extract FileChunk object
						std::stringstream ss;
						ss << m.getMessageBody();
						FileChunk fc;
						ss >> fc;

						// Write FileChunk to file
						_fileManager.addChunkToFile(fc);
					}
						break;
					case Message::FILE_CHUNK_REQUEST:
					{
						std::stringstream ss;
						std::string filename;
						int chunkIndex = 0;
						int totalChunks = 0;
						int dataSize = 0;

						ss << m.getMessageBody();
						ss >> filename;
						ss >> chunkIndex;
						ss >> totalChunks;
						ss >> dataSize;

						FileChunk fc(filename, chunkIndex, totalChunks, 0, 
							dataSize);

						if (!_fileManager.getChunkFromFile(fc)) {
							std::stringstream ss2;
							ss2 << fc;
							Message m(Message::FILE_CHUNK, ss2.str());
							(*it)->sendMessage(m);
						}
					}
						break;
					case Message::FILE_NOTIFICATION:
					{
						std::stringstream ss;
						ss << m.getMessageBody();

						File f;
						ss >> f;
						if (!_fileManager.exists(f.getFilename())) {
							_fileManager.addRemoteFile(f.getFilename(), 
								f.getTotalChunks(), f.getFileSize());
						}
					}
						break;
					default:
						break;
				}
			}

			// Send file chunk request
			std::map<std::string, File*>* ft = _fileManager.getFilesTable();
			std::map<std::string, File*>::iterator mit;

			for (mit = ft->begin(); mit != ft->end(); ++mit) {
				File* f = mit->second;
				if (f->getNumChunks() == f->getTotalChunks()) 
					continue;

				for (int i = 0; i < f->getTotalChunks(); ++i) {
					if (!f->isAvailable(i))
						continue;

					// Calculate chunk size
					int dataSize = constants::CHUNK_SIZE;
					if (i == f->getTotalChunks() - 1) {
						dataSize = ceil(
							(double)f->getFileSize() 
							/ (double)constants::CHUNK_SIZE);
					}

					std::stringstream ss;
					ss << f->getFilename() << " ";
					ss << i << " ";
					ss << f->getTotalChunks() << " ";
					ss << f->getFileSize() << " ";

					Message m(Message::FILE_CHUNK_REQUEST, ss.str());
					(*it)->sendMessage(m);
				}

			}
		}
	}
}
