/* * File: util.cpp
 * Group: 14
 * Description: Implementation for the Util class.
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include "util.h"

/*
 * Static function that extracts the filename from a given filepath. (i.e. given
 * dir1/dir2/file.txt, returns file.txt)
 *
 * filepath: The filepath containing the filename to be extracted
 */
std::string Util::extractFilename(std::string filepath) {
	int pos = filepath.find_last_of('/');

	pos = pos < 0 ? 0 : ++pos;
	return filepath.substr(pos);
}

/*
 * Checks whether the given path + filename is unique. If no file with the same
 * name exists then returns the given name. If name is not unique, then appends
 * an integer to the filename to make it unique (i.e. tmp.txt -> tmp(1).txt)
 *
 * folder: The folder where the file will be created
 * filename: The initial name of the file
 */
std::string Util::generateUniqueFilename(std::string folder, std::string filename) {
	long size;
	char* buf;
	char* ptr;

	size = pathconf(".", _PC_PATH_MAX);

	if ((buf = (char *)malloc((size_t)size)) != NULL)
		ptr = getcwd(buf, (size_t)size);
	
	std::stringstream ss;
	ss << ptr << "/" << folder << "/" << filename;
	return ss.str();
}

/*
 * Prints the given message
 */
void Log::info(std::string msg) {
	std::cout << "INFO: "  << msg << std::endl;
}

/*
 * Prints the given message
 */
void Log::warn(std::string msg) {
	std::cout << "WARN: "  << msg << std::endl;
}

/*
 * Prints the given message
 */
void Log::error(std::string msg) {
	std::cout << "ERROR: "  << msg << std::endl;
}
