/*
 * File: return_codes.h
 * Group: 14
 * Description: This flie contains a defined set of return codes.
 */

#ifndef _RETURN_CODES_
#define _RETURN_CODES_

namespace returnCodes {
	// Everything good
	const int OK =  0;

	// Uknown warning
	const int WARNING_UNKNOWN = 1;

	// Cannot find some peer; warning, since others may be connectable
	const int WARNING_PEER_NOT_FOUND = 5;

	// Number of peers exceed maximum; warning
	const int WARNING_TOO_MANY_PEERS = 6;

	// Unknown error
	const int ERROR_UNKNOWN = -2;

	// Cannot connect to anything; fatal error
	const int ERROR_CANNOT_CONNECT = -3;

	// Cannot find any peer (e.g. no peers in a peer file); fatal
	const int ERROR_NO_PEERS_FOUND = -4;
}

#endif /* _RETURN_CODES_ */
