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

const int chunkSize = 65536;
const std::string peersList = "peers.lst";
const std::string filesDir = "files";

// Forward declarations
class Status;

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

	private:
		Peers _peers;
};

#endif  /* _LOCAL_PEER_H_ */
