#include "socket.cpp"

using namespace std;

int main() {
	cout << "Hello from server" << endl;

	ServerSocket* s = new ServerSocket(8080);
	cout << "ServerSocket created" << endl;

	s->acceptConnection();

	cout << s->receiveData() << endl;

	delete s;

	return 0;
}
