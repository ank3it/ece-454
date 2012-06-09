/*
 * File: file.h
 * Group: 14
 * Description: Header for the File class. This class represents a file that is
 * being replicated across network. Manages IO for the given file in chunks.
 */

#ifndef _FILE_H_
#define _FILE_H_

#include <string>
#include "file_chunk.h"

class File {
	public:
		File();
		File(std::string, int, bool, int);
		~File();

		std::string getFilename() { return _filename; }
		int getNumChunks() { return _numChunks; }
		int getTotalChunks() { return _totalChunks; }
		bool isAvailable(int i) { return _isAvailable[i]; }
		bool* getIsAvailable() { return _isAvailable; }
		int getFileSize() { return _fileSize; }
		bool readChunk(FileChunk&);
		bool writeChunk(FileChunk&);

		std::string _filename;
		int _numChunks;
		int _totalChunks;
		bool* _isAvailable;
		int _fileSize;

	friend std::ostream& operator<<(std::ostream&, File const&);
	friend std::istream& operator>>(std::istream&, File&);
};

#endif /* _FILE_H_ */
