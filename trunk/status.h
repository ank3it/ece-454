/*
 * File: status.h
 * Date: May, 22/2012
 * Description: Status is the class that you populate with status data on the 
 * state of replication in this peer and its knowledge of the replication
 * level within the system.
 * The thing required in the Status object is the data as specified in the 
 * private section. The methods shown are examples of methods that we may 
 * implement to access such data. You will need to create methods to populate 
 * the Status data.
 */

#ifndef _STATUS_H_
#define _STATSU_H_

const int maxFiles  = 100;    // Cheesy, but allows us to do a simple Status class

class Status {
	public:
		int numberOfFiles();
		float fractionPresentLocally(int fileNumber);  // Use -1 to indicate if the file requested is not present
		float fractionPresent(int fileNumber);         // Use -1 to indicate if the file requested is not present in the system
		int   minimumReplicationLevel(int fileNumber); // Use -1 to indicate if the file requested is not present in the system
		float averageReplicationLevel(int fileNumber); // Use -1 to indicate if the file requested is not present in the system
		
	private:
		// This is very cheesy and very lazy, but the focus of this assignment
		// is not on dynamic containers but on the BT p2p file distribution

		int   _numFiles;                               // The number of files currently in the system, as viewed by this peer
		float _local[maxFiles];                        // The fraction of the file present locally 
													   // (= chunks on this peer/total number chunks in the file)
		float _system[maxFiles];                       // The fraction of the file present in the system 
													   // (= chunks in the system/total number chunks in the file)
													   // (Note that if a chunk is present twice, it doesn't get counted twice;
													   // this is simply intended to find out if we have the whole file in
													   // the system; given that a file must be added at a peer, think about why
													   // this number would ever not be 1.)
		int   _leastReplication[maxFiles];             // Sum by chunk over all peers; the minimum of this number is the least 
													   // replicated chunk, and thus represents the least level of replication
													   // of the file
		float _weightedLeastReplication[maxFiles];     // Sum all chunks in all peers; divde this by the number of chunks in the
													   // file; this is the average level of replication of the file
};

#endif  /* _STATUS_H_ */
