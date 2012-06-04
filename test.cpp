#include <iostream>
#include "peers.h"
#include "local_peer.h"

int main() {
	std::cout << "test.cpp" << std::endl;

	LocalPeer lp;
	lp.start();

	while (true);
}
