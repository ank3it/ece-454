/*
 * File: file_manager.h
 * Group: 14
 * Description: Header file for the FileManager class. This class is reponsible
 * for keeping track of all the files that this local peer has.
 */

#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#include <string>
#include <map>
#include "file.h"
#include "file_chunk.h"

class FileManager {
	public:
		FileManager() { /* Empty */ }
		~FileManager();

		static const int CHUNK_SIZE = 65536; // remove

		File* getFile(std::string);
		std::map<std::string, File*>* getFilesTable() { return &_filesTable; }
		bool exists(std::string filename);
		int addLocalFile(std::string);
		int addRemoteFile(std::string, int, int);
		void addChunkToFile(FileChunk&);
		bool getChunkFromFile(FileChunk&);

	private:
		std::map<std::string, File*> _filesTable;
};

#endif /* _FILE_MANAGER_H_ */
