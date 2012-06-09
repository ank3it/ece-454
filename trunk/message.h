#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

class Message {
	public:
		Message(); 
		Message(int, std::string);
		
		// Message types
		const static int UNKNOWN = 0;
		const static int LEAVE_NOTIFICATION = 1;
		const static int FILE_CHUNK = 2;
		const static int FILE_CHUNK_REQUEST = 3;
		const static int FILE_NOTIFICATION = 4;

		int getMessageType() { return _messageType; }
		std::string getMessageBody() { return _messageBody; }

	private:
		int _messageType;
		std::string _messageBody;
		
	friend std::ostream& operator<<(std::ostream&, Message const&);
	friend std::istream& operator>>(std::istream&, Message&);
};

#endif /* _MESSAGE_H_ */
