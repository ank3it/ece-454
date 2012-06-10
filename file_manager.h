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
#include <pthread.h>
#include "file.h"
#include "file_chunk.h"

class FileManager {
	public:
		FileManager();
		~FileManager();

		static const int CHUNK_SIZE = 65536; // remove

		File* getFile(std::string);
		std::map<std::string, File*>* getFilesTable() { return &_filesTable; }
		bool exists(std::string filename);
		int addLocalFile(std::string);
		int addRemoteFile(std::string, int, int);
		void addChunkToFile(FileChunk&);
		bool getChunkFromFile(FileChunk&);
		void lock();
		void unlock();

	private:
		std::map<std::string, File*> _filesTable;
		pthread_mutex_t _mutex;
};

#endif /* _FILE_MANAGER_H_ */
