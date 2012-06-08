/*
 * File: file.cpp
 * Group: 14
 * Description: Implementation for the File class which represents a file
 * being managed by the system.
 */

#include <fstream>
#include "file.h"
#include "constants.h"

/*
 * Constructor. Initializes the file object to the given value
 *
 * fn: The filename of the file
 * tc: The total number of chunks in the file
 * isAvailable: Whether the file is available locally (i.e. if we have any
 * chunks of the file available or not)
 */
File::File(std::string fn, int tc, bool isAvailable) 
: _filename(fn), _totalChunks(tc) {
	_isAvailable = new bool[_totalChunks];

	if (isAvailable) {
		// File available locally
		for (int i = 0; i < _totalChunks; i++)
			_isAvailable[i] = true;
	} else {
		// No chunks available locally
		for (int i = 0; i < _totalChunks; i++)
			_isAvailable[i] = false;
	}
}

/*
 * Destructor
 */
File::~File() {
	delete[] _isAvailable;
}

/*
 * Read in the specified chunk into the given buffer. Returns true if something
 * was read.
 *
 * chunkIndex: Zero-based index of the chunk to be read
 * buffer: The buffer in which the chunk data will be returned
 * size: The size of the data to be read
 */
bool File::readChunk(int chunkIndex, char* buffer, int size) {
	if (chunkIndex >= _totalChunks)
		return false;

	if (!_isAvailable[chunkIndex])
		return false;

	std::fstream inFile(_filename.c_str(), 
		std::ios::out | std::ios::binary | std::ios::app);

	if (!inFile.is_open())
		return false;

	int offset = chunkIndex * constants::CHUNK_SIZE;
	inFile.seekg(offset);

	inFile.read(buffer, size);
	inFile.close();

	return true;
}

/*
 * Write the buffer to the specified chunk. Returns true if something was 
 * written.
 *
 * chunkIndex: Zero-based index of the chunk to be written
 * buffer: The buffer that contains the chunk data
 * size: The size of the data to be written
 */
bool File::writeChunk(int chunkIndex, char* buffer, int size) {
	if (chunkIndex >= _totalChunks)
		return false;

	std::fstream outFile(_filename.c_str(), 
		std::ios::in | std::ios::binary | std::ios::app);

	if (!outFile.is_open())
		return false;

	int offset = chunkIndex * constants::CHUNK_SIZE;
	outFile.seekp(offset);

	outFile.write(buffer, size);
	outFile.close();

	// Mark chunk as available
	_isAvailable[chunkIndex] = true;

	return true;
}
