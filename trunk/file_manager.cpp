#include "StdAfx.h"
#include "file_manager.h"
#include "util.h"
#include <fstream>
#include "return_codes.h"


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
	
	std::ifstream inFile(filepath.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);
	
	if (inFile.is_open()) {
		//start from the beginning of the file
		inFile.seekg(0, std::ios::beg);
		
		for(int i = 0; !inFile.eof(); i++) {
			char* buffer;
			buffer = new char[chunkSize];
			inFile.read(buffer, chunkSize);
			FileChunk *chunk = new FileChunk(Util::extractFilename(filepath),i,0,buffer,chunkSize);
			nof_chunks = nof_chunks + 1;
			//TODO: how should I pass it on the totall no. of chunks
			//chunk->_totalChunks = nof_chunks;
			delete[] buffer;
		}
		inFile.close();
	} else {
		return -1;
	}

	return returnCodes::OK;
}