/*
 * File	local_peer.cpp
 * Group: 14
 * Description: Implementation for the LocalPeer class.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "local_peer.h"

LocalPeer::LocalPeer() {
	std::cout << "Peer constructor" << std::endl;
	// Empty constructor
}

LocalPeer::~LocalPeer() {
	// Empty destructor
}

int LocalPeer::insert(std::string filename) {
	return 0;
}

int LocalPeer::query(Status& status) {
	return 0;
}

int LocalPeer::join() {
	return 0;
}

int LocalPeer::leave() {
	return 0;
}
