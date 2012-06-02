#include <string>
#include "peer.h"

Peer::Peer() : _ipAddress(""), _portNumber(0), _state(unknown) {
	// Empty constructor
}

Peer::~Peer() {
	// Empty destructor
}
