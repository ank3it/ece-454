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
#include <iostream> // remove

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
int FileManager::addLocalFile(std::string filepath) {
	char* buffer;
	std::ifstream inFile(filepath.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	if (!inFile.is_open())
		return returnCodes::ERROR_FILE_NOT_FOUND;

	// Copy file
	// Read file into buffer
	std::ifstream::pos_type size = inFile.tellg();
	buffer = new char[size];

	inFile.seekg(0, std::ios::beg);
	inFile.read(buffer, size);

	// Write file from buffer
	std::string filename = Util::extractFilename(filepath);
	std::string outFilename = Util::generateUniqueFilename(
		constants::FILES_DIR, filename);

	std::ofstream outFile(outFilename.c_str(), 
		std::ios::out | std::ios::binary);

	inFile.close();

	if (!outFile.is_open())
		return returnCodes::ERROR_UNKNOWN;

	outFile.write(buffer, size);
	outFile.close();

	delete[] buffer;

	// Create File object and adds to files table
	int numberOfChunks = ceil((double)size / (double)constants::CHUNK_SIZE);
	File* file = new File(filename, numberOfChunks, true);
	_filesTable[filename] = file;

	return returnCodes::OK;
}

/*
 * Create a File object for the remote file and add it to the files table if it
 * does not already exist.
 */
int FileManager::addRemoteFile(std::string filepath, int numberOfChunks) {
	if (_filesTable.count(filepath) == 0) {
		File* file = new File(filepath, numberOfChunks, false);
		_filesTable[filepath] = file;
	}

	return returnCodes::OK;
}
