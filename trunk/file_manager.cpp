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
#include <sstream> // remove

FileManager::FileManager() {
	pthread_mutex_init(&_mutex, NULL);
}

/*
 * Destructor. Iterates through the files tables and delete all the dynamically 
 * allocated File objects.
 */
FileManager::~FileManager() {
	Log::info("FileManager destructor()");
	std::map<std::string, File*>::iterator it;

	for (it = _filesTable.begin(); it != _filesTable.end(); ++it) {
		delete it->second;
	}
}

/*
 * Returns a pointer to the given File object if it exists.
 *
 * filename: The requested File object.
 */
File* FileManager::getFile(std::string filename) {
	Log::info("in FileManager::getFile()");
	Log::info("getFile() filename = " + filename);
	if (exists(filename))
		return _filesTable[filename];
	else
		return NULL;
}

/*
 * Returns true if the given file exists in the files table.
 *
 * filename: The file to check for
 */
bool FileManager::exists(std::string filename) {
	return (_filesTable.count(filename) > 0);
}

/*
 * Copy the given file to the location where all replicated files are stored
 * and add it to the set of managed files.
 *
 * filepath: The path to the file to be added
 */
int FileManager::addLocalFile(std::string filepath) {
	Log::info("in FileManager::addLocalFile()");

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
	inFile.close();

	// Write file from buffer
	std::string filename = Util::extractFilename(filepath);
	std::string outFilename = Util::generateUniqueFilename(
		constants::FILES_DIR, filename);

	std::ofstream outFile(outFilename.c_str(), 
		std::ios::out | std::ios::binary);

	if (!outFile.is_open())
		return returnCodes::ERROR_UNKNOWN;

	outFile.write(buffer, size);
	outFile.close();

	delete[] buffer;

	// Create File object and adds to files table
	int numberOfChunks = ceil((double)size / (double)constants::CHUNK_SIZE);
	File* file = new File(filename, numberOfChunks, true, size);
	lock();
	_filesTable[filename] = file;
	unlock();

	return returnCodes::OK;
}

/*
 * Create a File object for the remote file and add it to the files table if it
 * does not already exist.
 */
int FileManager::addRemoteFile(std::string filepath, int numberOfChunks, int fileSize) {
	Log::info("in FileManager::addRemoteFile()");
	lock();
	if (_filesTable.count(filepath) == 0) {
		File* file = new File(filepath, numberOfChunks, false, fileSize);
		file->_numChunks = 0;

		_filesTable[filepath] = file;
	}
	unlock();

	return returnCodes::OK;
}

/*
 * Write the given chunk to file.
 *
 * fc: A filled FileChunk object
 */
void FileManager::addChunkToFile(FileChunk& fc) {
	Log::info("FileManager::addChunkToFile()");
	lock();
	if (_filesTable.count(fc.getFilename()) > 0) {
		if (!_filesTable[fc.getFilename()]->isAvailable(fc.getChunkIndex()))
			_filesTable[fc.getFilename()]->writeChunk(fc);
	}
	unlock();
}

/*
 * Read a chunk from file.
 *
 * fc: A partly filled FileChunk object
 */
bool FileManager::getChunkFromFile(FileChunk& fc) {
	Log::info("FileManager::getChunkFromFile()");
	lock();
	if (_filesTable.count(fc.getFilename()) == 0) {
		unlock();
		return false;
	}
	
	_filesTable[fc.getFilename()]->readChunk(fc);
	unlock();
	return true;
}

/*
 * Lock the class to synchronize access across threads.
 */
void FileManager::lock() {
	pthread_mutex_lock(&_mutex);
}

/*
 * Unlock the class.
 */
void FileManager::unlock() {
	pthread_mutex_unlock(&_mutex);
}
