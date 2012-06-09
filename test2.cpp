#include <iostream>
#include "file.h"
#include <sstream>

int main() {
	std::cout << "test2.cpp" << std::endl;

	File f("tmp.txt", 5, true);
	std::stringstream ss;
	ss << f;
	std::cout << ss.str() << std::endl;

	File newf;
	std::cout << newf << std::endl;
	ss >> newf;
	std::cout << newf << std::endl;

	return 0;
}
