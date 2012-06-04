/*
 * File local_peer.h
 * Group: 14
 * Description: Header file for the LocalPeer class which represents the machine
 * on which this code is running.
 */

#ifndef _LOCAL_PEER_H_
#define _LOCAL_PEER_H_

#include <string>
#include "peers.h"
#include "socket/socket.h"

const int CHUNK_SIZE = 65536;
const unsigned short PORT_NUMBER = 8080;
const std::string PEERS_LIST = "peers.lst";
const std::string FILES_DIR = "files";

// Forward declarations
class Status;

class LocalPeer : public Thread {
	public:
		LocalPeer();
		~LocalPeer();

		// Required interface
		int insert(std::string filename);
		int query(Status& status);
		int join();
		int leave();
		// End of required interface

		void run();

	private:
		Peers _peers;
		ServerSocket* _serverSocket;
};

#endif  /* _LOCAL_PEER_H_ */
