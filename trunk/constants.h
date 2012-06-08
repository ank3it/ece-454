/*
 * File: constants.h
 * Group: 14
 * Description: Constains constants that are used in different places throughout
 * the code.
 */

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>

namespace constants {
	const int CHUNK_SIZE = 65357;
	const int MAX_PEERS = 6;
	const std::string PEERS_LIST = "peers.lst";
	const std::string FILES_DIR = "yo";
	const unsigned short DEFAULT_PORT_NUMBER = 8080;
}

#endif /* _CONSTANTS_H_ */
