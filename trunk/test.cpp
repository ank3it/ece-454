#include <iostream>
#include <sstream>
#include "file_chunk.h"
#include "message.h"

int main() {
	std::cout << "test.cpp" << std::endl;

	std::stringstream ss;
	ss << "this test";
	std::string s;
	ss >> s;
	std::cout << s << std::endl;
	ss >> s;
	std::cout << s << std::endl;

	return 0;
}
