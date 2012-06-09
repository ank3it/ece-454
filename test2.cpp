#include <iostream>
#include <sstream>
#include "local_peer.h"
#include "util.h"

int main() {
	std::cout << "test2.cpp" << std::endl;

	LocalPeer lp(8081);
	Log::info("LocalPeer created");
	lp.join();
	Log::info("Joined network");
	lp.leave();
	Log::info("Left network");

	Log::info("at infinite loop");
	while (true);

	return 0;
}
