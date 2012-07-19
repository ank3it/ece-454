#include "message.h"
#include <iostream>
#include <cstdlib>

Message::Message() : 
_messageType(Message::UNKNOWN), 
_messageBody("") {
	// Empty
}

Message::Message(int mt, std::string mb) :	
_messageType(mt), 
_messageBody(mb) {
	// Empty
		std::cout << std::endl << "In Message " << _messageBody.length() << std::endl;
}

std::ostream& operator<<(std::ostream& os, Message const& msg) {
	os << "_START_MESSAGE_ ";
	os << "_MESSAGE_TYPE_ " << msg._messageType << " ";
	os << "_MESSAGE_SIZE_ " << msg._messageBody.length() << " ";
	os << "_START_MESSAGE_BODY_ ";
	//char *buf = new char[msg._messageBody.length() + 1];
	//buf = msg._messageBody.c_str();
	for ( int i = 0; i < msg._messageBody.length() ; i++ )
	os.put(msg._messageBody.c_str()[i]); 
	//delete[] buf;
	os << "_END_MESSARANDGE_BODY_ ";
	os << "_END_MESSAGE_ ";
	os.flush();
	return os;
}

std::istream& operator>>(std::istream& is, Message& msg) {
	std::string tmpStr;
	
	int message_size = 100;

	is >> tmpStr;
	std::cout << "Tmp Str " << tmpStr << std::endl;
	if (tmpStr.compare("_START_MESSAGE_") == 0) {
	//	do {
			is >> tmpStr;
			std::cout << "Tmp Str " << tmpStr << std::endl;
			if (tmpStr.compare("_MESSAGE_TYPE_") == 0) {
				is >> msg._messageType;
				std::cout << "msg._messageType " << msg._messageType << std::endl;
			}  
			
			is >> tmpStr;
			std::cout << "Tmp Str " << tmpStr << std::endl;
			if (tmpStr.compare("_MESSAGE_SIZE_") == 0) {
				is >> message_size;
				std::cout << " message_size " <<  message_size << std::endl;
			}
			
			is >> tmpStr;
			std::cout << "Tmp Str " << tmpStr << std::endl;
			
			char *buf = new char[message_size];
		//	if (tmpStr.compare("_START_MESSAGE_BODY_") == 0) {
				//std::string msgBody;
				is.read(buf, message_size);
				std::cout << " Size Read " << is.gcount() << std::endl;
				for ( int i = 0 ; i < message_size; i++ ) {
					msg._messageBody  += buf[i];
					}
				//std::string str(buf);
				//std::cout << "str " << str.length() << std::endl;
				//msg._messageBody = str;
				delete[] buf;
				//is >> tmpStr;
				is >> tmpStr;
				std::cout << "Tmp Str1 " << tmpStr << std::endl;
				is >> tmpStr;
				std::cout << "Tmp Str2 " << tmpStr << std::endl;

				//while (tmpStr.compare("_END_MESSARANDGE_BODY_") != 0) {
				   // is.readsome(tmpstr, 65357);
					//msgBody += buf;
					//msgBody += " ";
					//is >> tmpStr;

					//if (tmpStr.compare("_END_MESSARANDGE_BODY_") != 0)
					//	msgBody += " ";
			//	}

				//msg._messageBody = msgBody;
				std::cout << "msgBody " << msg._messageBody.length() << std::endl;
		//	}
		//} while (tmpStr.compare("_END_MESSAGE_") != 0);
	} else {
		// Reset the stream get pointer position
		is.seekg(-tmpStr.length(), std::ios_base::cur);
	}
	
	

	return is;
}
