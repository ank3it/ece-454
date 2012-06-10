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
		FileChunk(std::string, int, int, int);
		~FileChunk();

		std::string getFilename() { return _filename; }
		int getChunkIndex() { return _chunkIndex; }
		int getTotalChunks() { return _totalChunks; }
		int getDataSize() { return _dataSize; }
		char* getData() { return _data; }

	private:
		std::string _filename;
		int _chunkIndex;
		int _totalChunks;
		int _dataSize;		// Size of chunk, will usually equal chunk size
		char* _data;		// Holds the file chunk data

	friend std::ostream& operator<<(std::ostream&, FileChunk const&);
	friend std::istream& operator>>(std::istream&, FileChunk&);
};

#endif /* _FILE_CHUNK_H_ */
