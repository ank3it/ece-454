#include "file_manager.h"
#include "util.h"
#include <fstream>
#include "return_codes.h"
#include <list>
#include <string>

file_manager::file_manager(void)
{
}


file_manager::~file_manager(void)
{
}


/*
 * Splits the files into small chunks
 * chunkSize determines the size of each size
 */
int file_manager::chunk_file(std::string filepath) {
	
	int nof_chunks;
	std::list<std::string> list;
	std::list<std::string>::iterator it;
	
	std::ifstream inFile(filepath.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);
	
	if (inFile.is_open()) {
		//start from the beginning of the file
		inFile.seekg(0, std::ios::beg);
		
		for(int i = 0; !inFile.eof(); i++) {
			char* buffer;
			buffer = new char[chunkSize];
			inFile.read(buffer, chunkSize);
			list.push_front(buffer);
			delete[] buffer;
		}
		
		for ( int i=0; list.size() != 0 ;i++)
		{
			std::string data = list.front();
			//facing an error in the line below
			FileChunk *chunk = new FileChunk((Util::extractFilename(filepath)),i,list.size(),data.c_str(),chunkSize);
		}

		inFile.close();
	} else {
		return -1;
	}

	return returnCodes::OK;
}