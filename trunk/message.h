#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

class Message {
	public:
		Message(); 
		Message(std::string, int, int, std::string);
		
		const static int UNKNOWN = 0;
		const static int JOIN_NOTIFICATION = 1;
		const static int LEAVE_NOTIFICATION = 2;
		const static int FILE_CHUNK = 3;
		const static int FILE_CHUNK_REQUEST = 4;

		std::string _senderIpAddress;
		int _senderPortNumber;
		int _messageType;
		std::string _messageBody;
		
	friend std::ostream& operator<<(std::ostream&, Message const&);
	friend std::istream& operator>>(std::istream&, Message&);
};

#endif /* _MESSAGE_H_ */
