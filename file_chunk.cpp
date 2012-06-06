/*
 * File: file_chunk.cpp
 * Group: 14
 * Description: Implementation of the FileChunk class which represents a small
 * portion of a file.
 */

#include "file_chunk.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

/*
 * Default constructor, initializes all values to 0/NULL/""
 */
FileChunk::FileChunk() 
: _filename(""), _chunkIndex(0), _totalChunks(0), _data(0), _dataSize(0) {
	// Empty constructor
}

/*
 * Constructor. Initializes class with given values. 
 */
FileChunk::FileChunk(std::string fn, int ci, int tc, char d[], int ds) 
: _filename(fn), _chunkIndex(ci), _totalChunks(tc), _data(d), _dataSize(ds) {
	_data = new char[ds];
	memcpy(_data, d, ds);
}

/*
 * Destructor. Delete the dynamically allocated _data member which holds the 
 * chunk data
 */
FileChunk::~FileChunk() {
	delete[] _data;
}

/*
 * Returns a text-based serialization of the given FileChunk object
 *
 * os: The output stream object (e.g. ofstream or ostringstream)
 * fileChunk: the FileChunk object to serialize
 */
std::ostream& operator<<(std::ostream& os, FileChunk const& fileChunk) {
	os << "_START_FILECHUNK_ ";
	os << "_FILENAME_ " << fileChunk._filename << " ";
	os << "_CHUNKINDEX_ " << fileChunk._chunkIndex << " ";
	os << "_TOTALCHUNKS_ " << fileChunk._totalChunks << " ";
	os << "_DATASIZE_ " << fileChunk._dataSize << " ";
	os << "_START_DATA_ ";
	os.write(fileChunk._data, fileChunk._dataSize);
	os << " _END_DATA_ ";
	os << "_END_FILECHUNK_ ";

	return os;
}

/*
 * Deserialize the FileChunk object from the given input stream
 *
 * is: The input stream (e.g. ifstream, istringstream)
 * fileChunk: The FileChunk object into which the data will be deserialized
 */
std::istream& operator>>(std::istream& is, FileChunk& fileChunk) {
	std::string tmpStr;

	is >> tmpStr;
	if (tmpStr.compare("_START_FILECHUNK_") == 0) {
		do {
			is >> tmpStr;

			if (tmpStr.compare("_FILENAME_") == 0) {
				is >> fileChunk._filename;
			} else if (tmpStr.compare("_CHUNKINDEX_") == 0) {
				is >> fileChunk._chunkIndex;
			} else if (tmpStr.compare("_TOTALCHUNKS_") == 0) {
				is >> fileChunk._totalChunks;
			} else if (tmpStr.compare("_DATASIZE_") == 0) {
				is >> fileChunk._dataSize;
				fileChunk._data = new char[fileChunk._dataSize];
			} else if (tmpStr.compare("_START_DATA_") == 0) {
				// Get and discard the leading space
				is.ignore();

				// Get the actual data
				is.read(fileChunk._data, fileChunk._dataSize);
			}
	
		} while (tmpStr.compare("_END_FILECHUNK_") != 0);
	} else {
		// Reset the stream get pointer position
		is.seekg(-tmpStr.length(), std::ios_base::cur);
	}

	return is;
}
