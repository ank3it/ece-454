#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "local_peer.h"
#include "util.h"
#include "file.h"
#include "file_chunk.h"

int main() {
	std::cout << "test.cpp" << std::endl;

	LocalPeer lp;
	lp.join();
	Log::info("joined");
	lp.insert("/home/usman/working_dir/p1/in.txt");
	Log::info("inserted");

	Log::info("at infinite while loop");

	std::string str;
	while (str.compare("q") != 0) {
		std::cin >> str;
	}
	Log::info("after infinite while loop");

/*	
	File f("in.jpg", 12, true, 737937);
	FileChunk* fc[12];
	fc[0] = new FileChunk("in.jpg", 0, 12, 65357);
	fc[1] = new FileChunk("in.jpg", 1, 12, 65357);
	fc[2] = new FileChunk("in.jpg", 2, 12, 65357);
	fc[3] = new FileChunk("in.jpg", 3, 12, 65357);
	fc[4] = new FileChunk("in.jpg", 4, 12, 65357);
	fc[5] = new FileChunk("in.jpg", 5, 12, 65357);
	fc[6] = new FileChunk("in.jpg", 6, 12, 65357);
	fc[7] = new FileChunk("in.jpg", 7, 12, 65357);
	fc[8] = new FileChunk("in.jpg", 8, 12, 65357);
	fc[9] = new FileChunk("in.jpg", 9, 12, 65357);
	fc[10] = new FileChunk("in.jpg", 10, 12, 65357);
	fc[11] = new FileChunk("in.jpg", 11, 12, 19010);
	f.readChunk(*fc[0]);
	f.readChunk(*fc[1]);
	f.readChunk(*fc[2]);
	f.readChunk(*fc[3]);
	f.readChunk(*fc[4]);
	f.readChunk(*fc[5]);
	f.readChunk(*fc[6]);
	f.readChunk(*fc[7]);
	f.readChunk(*fc[8]);
	f.readChunk(*fc[9]);
	f.readChunk(*fc[10]);
	f.readChunk(*fc[11]);
	
	Log::info("read in all the chunks");
	
	//std::ifstream ifs("in.txt", std::ios::binary);
	//char buf[13];
	//ifs.read(buf, 13);
	

	f.writeChunk(*fc[1]);
	f.writeChunk(*fc[2]);
		f.writeChunk(*fc[6]);
	f.writeChunk(*fc[3]);
	f.writeChunk(*fc[4]);

	f.writeChunk(*fc[7]);
		f.writeChunk(*fc[5]);
		f.writeChunk(*fc[0]);
	f.writeChunk(*fc[8]);
	f.writeChunk(*fc[9]);
		f.writeChunk(*fc[11]);
	f.writeChunk(*fc[10]);
	
	for (int i = 0; i < 12; i++) {
		//f.writeChunk(*fc[i]);
	}
	//f.writeChunk(fc0);
	//f.writeChunk(fc1);

	for (int i = 11; i >= 0; i--) {
		delete fc[i];
	}
	*/

	return 0;
}
