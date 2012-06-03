/*
 * File: peers.h
 * Date: May, 22/2012
 * Description: Peers is a dumb container to hold the peers; the number of 
 * peers is fixed, but needs to be set up when a peer starts up; feel free to 
 * use some other container class, but that class must have a method that 
 * allows it to read the peersFile, since otherwise you have no way of having 
 * a calling entity tell your code what the peers are in the system.
 */

#ifndef _PEERS_H_
#define _PEERS_H_

#include <string>
#include "peer.h"

const int maxPeers  = 6;

class Peers {
	public:
		Peers();

		// The peersFile is the name of a file that contains a list of the 
		// peers. Its format is as follows: in plaintext there are up to 
		// maxPeers lines, where each line is of the form: 
		// <IP address> <port number>
		// This file should be available on every machine on which a peer is 
		// started, though you should exit gracefully if it is absent or 
		// incorrectly formatted. After execution of this method, the _peers 
		// should be present.
		int initialize(std::string peersFile);
		Peer& operator()(int i);		
		int getNumPeers() const { return _numPeers; }
		
	private:
		int _numPeers;
		Peer _peers[maxPeers];
};

#endif  /* _PEERS_H_ */
