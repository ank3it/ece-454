/* * File: file_chunk.h * Group: 14
 * Description: Header file for the FileChunk class. This class represents a 
 * small chunk of a larger file. Overloads the << and >> operators to provide
 * text-based serialization and deserialization.
 */

#ifndef _FILE_CHUNK_H_
#define _FILE_CHUNK_H_

#include <string>

class FileChunk {
	public:
		FileChunk();
		FileChunk(std::string, int, int, char*, int);
		~FileChunk();

		std::string _filename;
		int _chunkIndex;
		int _totalChunks;
		int _dataSize;		// Size of chunk, will usually be chunkSize
		char* _data;		// Holds the file chunk data

	friend std::ostream& operator<<(std::ostream&, FileChunk const&);
	friend std::istream& operator>>(std::istream&, FileChunk&);
};

#endif /* _FILE_CHUNK_H_ */
