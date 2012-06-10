/*
 * File: file.cpp
 * Group: 14
 * Description: Implementation for the File class which represents a file
 * being managed by the system.
 */

#include <fstream>
#include "file.h"
#include "constants.h"
#include "util.h"
#include <iostream> // remove

File::File() : 
_filename(""), _numChunks(0), _totalChunks(0), _isAvailable(0), _fileSize(0) {
	// Empty
}

/*
 * Constructor. Initializes the file object to the given value
 *
 * fn: The filename of the file
 * tc: The total number of chunks in the file
 * isAvailable: Whether the file is available locally (i.e. if we have any
 * chunks of the file available or not)
 */
File::File(std::string fn, int tc, bool isAvailable, int fs) 
: _filename(fn), _numChunks(tc), _totalChunks(tc), _fileSize(fs) {
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
 * fc: A FileChunk object with data such as chunk index and data size filled in.
 * The read data will also be returned in this object.
 */
bool File::readChunk(FileChunk& fc) {
	Log::info("File::readChunk()");
	Log::info("_filename = " + _filename);
	if (fc.getChunkIndex() >= _totalChunks)
		return false;

	if (!_isAvailable[fc.getChunkIndex()])
		return false;
		
	Log::info("still here");
	
	std::string filepath = constants::FILES_DIR + "/" + _filename;

	std::ifstream inFile(filepath.c_str(), std::ios::out|std::ios::binary);

	if (!inFile.is_open())
		return false;

	int offset = fc.getChunkIndex() * constants::CHUNK_SIZE;
	inFile.seekg(offset, std::ios::beg);

	inFile.read(fc.getData(), fc.getDataSize());
	inFile.close();

	return true;
}

/*
 * Write the buffer to the specified chunk. Returns true if something was 
 * written.
 *
 * chunk: A FileChunk object containing the data to be written
 */
bool File::writeChunk(FileChunk& fc) {
	Log::info("File::writeChunk()");
	
	static bool isWrittenToDisk = false;
	static int callCounter = 0;
	
	if (fc.getChunkIndex() >= _totalChunks)
		return false;
	
	std::ofstream outFile;
	std::string filepath = constants::FILES_DIR + "/" + _filename;
	
	// Create a sparse file for writting the chunks to	
	if (!isWrittenToDisk) {
		outFile.open(filepath.c_str(), std::ios::binary);
		outFile.seekp(_fileSize - 1);
		outFile.write(" ", 1);
		outFile.close();
		isWrittenToDisk = true;
	}	

	outFile.open(filepath.c_str(), std::ios::in|std::ios::out|std::ios::binary);

	if (!outFile.is_open())
		return false;

	long offset = fc.getChunkIndex() * constants::CHUNK_SIZE;
	outFile.seekp(offset);

	outFile.write(fc.getData(), fc.getDataSize());
	outFile.close();

	// Mark chunk as available
	_isAvailable[fc.getChunkIndex()] = true;
	_numChunks++;
	
	Log::info("writeChunk() completed");
	callCounter++;
	std::cout << "callCounter = " << callCounter << std::endl;

	return true;
}

std::ostream& operator<<(std::ostream& os,  File const& file) {
	os << "_START_FILE_ ";
	os << "_FILENAME_ " << file._filename << " ";
	os << "_TOTAL_CHUNKS_ " << file._totalChunks << " ";
	os << "_IS_AVAILABLE_ ";
	for (int i = 0; i < file._totalChunks; ++i) {
		os  << file._isAvailable[i] << " ";
	}
	os << "_FILE_SIZE_ " << file._fileSize << " ";
	os << "_END_FILE_ ";

	return os;
}

std::istream& operator>>(std::istream& is, File& file) {
	std::string str;

	is >> str;
	if (str.compare("_START_FILE_") == 0) {
		do {
			is >> str;

			if (str.compare("_FILENAME_") == 0) {
				is >> file._filename;
			} else if (str.compare("_TOTAL_CHUNKS_") == 0) {
				is >> file._totalChunks;
			} else if (str.compare("_IS_AVAILABLE_") == 0) {
				file._isAvailable = new bool[file._totalChunks];
				for (int i = 0; i < file._totalChunks; ++i) {
					is >> file._isAvailable[i];
				}
			} else if (str.compare("_FILE_SIZE_") == 0) {
				is >> file._fileSize;
			}
		} while (str.compare("_END_FILE_") != 0);
	} else {
		// Reset stream get pointer position
		is.seekg(-str.length(), std::ios::cur);
	}

	return is;
}
