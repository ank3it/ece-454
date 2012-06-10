#include <iostream>
#include <sstream>
#include "local_peer.h"
#include "util.h"

int main() {
	std::cout << "test2.cpp" << std::endl;

	LocalPeer lp(8082);
	Log::info("LocalPeer created");
	lp.join();
	Log::info("Joined network");

	Log::info("at infinite loop");
	std::string str;
	while (str.compare("q") != 0) {
		std::cin >> str;
	}
	Log::info("after infinite while loop");

	return 0;
}
