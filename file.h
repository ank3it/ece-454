/*
 * File: file.h
 * Group: 14
 * Description: Header for the File class. This class represents a file that is
 * being replicated across network. Manages IO for the given file in chunks.
 */

#ifndef _FILE_H_
#define _FILE_H_

#include <string>

class File {
	public:
		File(std::string, int, bool);
		~File();

		bool readChunk(int, char*, int);
		bool writeChunk(int, char*, int);

	private:
		std::string _filename;
		int _totalChunks;
		bool* _isAvailable;
};

#endif /* _FILE_H_ */
