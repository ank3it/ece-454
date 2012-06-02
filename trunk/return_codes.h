/*
 * File: return_codes.h
 * Group: 14
 * Description: This flie contains a defined set of return codes.
 */

#ifndef _RETURN_CODES_
#define _RETURN_CODES_

namespace returnCodes {
	const int errOK             =  0; // Everything good
	const int errUnknownWarning =  1; // Unknown warning
	const int errUnknownFatal   = -2; // Unknown error
	const int errCannotConnect  = -3; // Cannot connect to anything; fatal error
	const int errNoPeersFound   = -4; // Cannot find any peer (e.g., no peers in a peer file); fatal
	const int errPeerNotFound   =  5; // Cannot find some peer; warning, since others may be connectable

	const int errTooManyPeers	=  6; // Number of peers exceeds maximum
}

#endif /* _RETURN_CODES_ */
