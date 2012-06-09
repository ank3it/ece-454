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
#include "server.h"
#include "file_manager.h"

// Forward declarations
class Status;

class LocalPeer : public Thread {
	public:
		LocalPeer();
		LocalPeer(unsigned short);
		~LocalPeer();

		// Required interface
		int insert(std::string filename);
		int query(Status& status);
		int join();
		int leave();
		// End of required interface

		void broadcastFileNotification(std::string);
		void broadcastLeaveNotification();

	private:
		unsigned short _portNumber;
		Peers _peers;
		ServerSocket* _serverSocket;
		Server* _server;
		FileManager _fileManager;

		void run();
};

#endif  /* _LOCAL_PEER_H_ */
