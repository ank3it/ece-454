#include <iostream>
#include <sstream>
#include "file_chunk.h"
#include "message.h"

int main() {
	std::cout << "test.cpp" << std::endl;

	std::stringstream ss;
	FileChunk fc("tmp.txt", 2, 24, "hello", 5);
	std::cout << fc << std::endl << std::endl;
	ss << fc;

	Message m("127.0.0.1", 8080, Message::FILE_CHUNK, ss.str());
	std::stringstream ss2;
	ss2 << m;
	std::cout << ss2.str() << std::endl << std::endl;

	Message n;
	ss2 >> n;

	std::cout << n << std::endl << std::endl;

	FileChunk fc2;
	std::stringstream ss3;
	ss3 << n._messageBody;
	std::cout << ss3.str() << std::endl << std::endl;

	ss3 >> fc2;
	std::cout << fc2 << std::endl;

	return 0;
}
