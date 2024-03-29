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
#include "message.h"
#include "socket/socket.h"

class Peer : public Thread {
	public:
		Peer();
		~Peer();

		enum State { connected, disconnected, unknown };

		State getState() const { return _state; }
		ConcurrentQueue<Message>* getReceiveQueue() { return &_receiveQueue; }
		void setIpAddress(std::string const ipAddress) { _ipAddress = ipAddress; }
		void setPortNumber(int const value) { _portNumber = value; }
		void setState(State const value) { _state = value; }
		void setSocket(Socket const value) { _socket = value; }
		bool connect();
		void disconnect();
		void sendMessage(Message&);

	private:
		std::string _ipAddress;
		int _portNumber;
		State _state;
		pthread_t _threadId;
		ConcurrentQueue<Message> _receiveQueue;
		Socket _socket;

		void run();
};

#endif  /* _PEER_H_ */
