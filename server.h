#ifndef _SERVER_H_
#define _SERVER_H_

#include "thread.h"
#include "socket/socket.h"
#include "peers.h"

class Server : public Thread {
	public:
		Server();
		Server(ServerSocket*, Peers*);
		~Server();

		bool startServer();
		bool stopServer();

	private:
		ServerSocket* _serverSocket;
		Peers* _peers;

		void run();
};

#endif /* _SERVER_H_ */
