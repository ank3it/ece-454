/*
 * File peer.h
 * Group: 14
 * Description: Header file for the Peer class which represents the remote peers
 * in the network.
 */

#ifndef _PEER_H_
#define _PEER_H_

#include <string>
#include "thread.h"
#include "queue/concurrent_queue.h"
#include "file_chunk.h"
#include "socket/socket.h"

class Peer : public Thread {
	public:
		Peer();
		~Peer();

		enum State { connected, disconnected, unknown };

		State getState() const { return _state; }
		void setIpAddress(std::string const ipAddress) { _ipAddress = ipAddress; }
		void setPortNumber(int const portNumber) { _portNumber = portNumber; }
		void setState(State newState) { _state = newState; }
		bool connect();
		void run();

	private:
		std::string _ipAddress;
		int _portNumber;
		enum State _state;
		pthread_t _threadId;
		ConcurrentQueue<FileChunk> _receiveQueue;
		ConcurrentQueue<FileChunk> _sendQueue;
		Socket socket;
};

#endif  /* _PEER_H_ */
