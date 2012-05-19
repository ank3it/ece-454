#include "socket.cpp"

using namespace std;

int main() {
	string hostName = "localhost";

	cout << "Hello from client" << endl;

	ClientSocket* cSock = new ClientSocket(hostName, 8080);

	cout << "ClientSocket created" << endl;

	string msg;
	getline(cin, msg);

	cSock->sendData(msg, msg.length());

	delete cSock;

	return 0;
}
