#ifndef _FILE_H_
#define _FILE_H_

#include <string>
#include <fstream>

class File {
	public:
		File();
		File(std::string, int, bool);
		~File();

		bool open();
		void close();
		bool readChunk(int, char*, int);
		bool writeChunk(int, char*, int);

	private:
		std::string _filename;
		std::fstream _file;
		int _totalChunks;
		bool* _isChunkAvailable;
};

#endif /* _FILE_H_ */
