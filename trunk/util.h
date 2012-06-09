/*
 * File: util.h
 * Group: 14
 * Description: Header for the Util class. This class contains common utility
 * functions.
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>

class Util {
	public:
		static std::string extractFilename(std::string);
		static std::string generateUniqueFilename(std::string, std::string);
};

class Log {
	public:
		static void info(std::string);
		static void warn(std::string);
		static void error(std::string);
};

#endif /* _UTIL_H_ */
