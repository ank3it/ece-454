/*
 * File: file_manager.cpp
 * Group: 14
 * Description: Implementation of the FileManager class
 */

#include <fstream>
#include <cmath>
#include "file_manager.h"
#include "return_codes.h"
#include "constants.h"
#include "util.h"
#include "file_chunk.h"

/*
 * Destructor. Iterates through the files tables and delete all the dynamically 
 * allocated File objects.
 */
FileManager::~FileManager() {
	std::map<std::string, File*>::iterator it;

	for (it = _filesTable.begin(); it != _filesTable.end(); ++it) {
		delete it->second;
	}
}

/*
 * Copy the given file to the location where all replicated files are stored
 * and add it to the set of managed files.
 *
 * filepath: The path to the file to be added
 */
int FileManager::addFile(std::string filepath) {
}
