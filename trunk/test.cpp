#include <iostream>
#include <sstream>
#include <string>
#include "local_peer.h"
#include "util.h"

int main() {
	std::cout << "test.cpp" << std::endl;

	LocalPeer lp;
	lp.join();
	Log::info("joined");
	lp.insert("/home/usman/working_dir/p1/in.jpg");
	Log::info("inserted");

	Log::info("at infinite while loop");

	std::string str;
	while (str.compare("q") != 0) {
		std::cin >> str;
	}
	Log::info("after infinite while loop");

	return 0;
}
