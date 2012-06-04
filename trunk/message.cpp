#include "message.h"
#include <iostream>
#include <cstdlib>

Message::Message() : 
_senderIpAddress(""), 
_senderPortNumber(0), 
_messageType(Message::UNKNOWN), 
_messageBody("") {
	// Empty
}

Message::Message(std::string ipa, int pn, int mt, std::string mb) :	
_senderIpAddress(ipa), 
_senderPortNumber(pn), 
_messageType(mt), 
_messageBody(mb) {
	// Empty
}

std::ostream& operator<<(std::ostream& os, Message const& msg) {
	os << "_START_MESSAGE_ ";
	os << "_SENDER_IP_ADDRESS_ " << msg._senderIpAddress << " ";
	os << "_SENDER_PORT_NUMBER_ " << msg._senderPortNumber << " ";
	os << "_MESSAGE_TYPE_ " << msg._messageType << " ";
	os << "_START_MESSAGE_BODY_ " << msg._messageBody << " _END_MESSAGE_BODY_ ";
	os << "_END_MESSAGE_ ";

	return os;
}

std::istream& operator>>(std::istream& is, Message& msg) {
	std::string tmpStr;

	is >> tmpStr;
	if (tmpStr.compare("_START_MESSAGE_") == 0) {
		do {
			is >> tmpStr;

			if (tmpStr.compare("_SENDER_IP_ADDRESS_") == 0) {
				is >> tmpStr;
				msg._senderIpAddress = tmpStr;
			} else if (tmpStr.compare("_SENDER_PORT_NUMBER_") == 0) {
				is >> tmpStr;
				msg._senderPortNumber = atoi(tmpStr.c_str());
			} else if (tmpStr.compare("_MESSAGE_TYPE_") == 0) {
				is >> tmpStr;
				msg._messageType = atoi(tmpStr.c_str());
			} else if (tmpStr.compare("_START_MESSAGE_BODY_") == 0) {
				std::string msgBody;
				is >> tmpStr;

				while (tmpStr.compare("_END_MESSAGE_BODY_") != 0) {
					msgBody += tmpStr;
					is >> tmpStr;

					if (tmpStr.compare("_END_MESSAGE_BODY_") != 0)
						msgBody += " ";
				}

				msg._messageBody = msgBody;
			}
		} while (tmpStr.compare("_END_MESSAGE_") != 0);
	} else {
		// Reset the stream get pointer position
		is.seekg(-tmpStr.length(), std::ios_base::cur);
	}

	return is;
}
