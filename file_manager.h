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

class FileManager {
	public:
		FileManager() { /* Empty */ }
		~FileManager();

		static const int CHUNK_SIZE = 65536; // remove

		int addLocalFile(std::string);
		int addRemoteFile(std::string, int);

	private:
		std::map<std::string, File*> _filesTable;
};

#endif /* _FILE_MANAGER_H_ */
