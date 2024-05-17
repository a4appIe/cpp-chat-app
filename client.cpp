#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define PORT 8080

bool init() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

//Sending Data continuously
void sendMsg(SOCKET s) {
	while (true) {
		string name;
		string msg;
		cout << "Enter your name: ";
		getline(cin, name);
		cout << endl;
		while (true) {
			cout << "::  ";
			getline(cin, msg);
			int bytesSend = send(s, msg.c_str(), msg.length(), 0);
			int nameBytes = send(s, name.c_str(), name.length(), 0);
			if (bytesSend == SOCKET_ERROR) {
				cout << "Error while sending message :(" << endl;
				break;
			}
			if (msg == "/quit") {
				cout << "Stopping the chat..." << endl << "See you again..." << endl << endl;
				break;
				return;
			}
		}
	}
	closesocket(s);
	WSACleanup();
}

//Receiving Data continuously
void receiveMsg(SOCKET s) {
	char buffer[4096];
	int recvLen;
	char name[1024];
	int nameLen;
	string msg = "";
	string userName = "";
	while (true) {
		recvLen = recv(s, buffer, sizeof(buffer), 0);
		nameLen = recv(s, name, sizeof(name), 0);
		if (recvLen <= 0) {
			cout << "Disconnected from the server!" << endl;
			break;
		}
		else {
			msg = string(buffer, recvLen);
			userName = string(name, nameLen);
			cout << "From " << userName << ": " << msg << endl << "::  ";
		}
	}
	closesocket(s);
	WSACleanup();
}

int main() {
	if (!init()) {
		cout << "Winsock init failed!" << endl;
		return 1;
	}

	//Socket creation
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "Invalid socket created!" << endl;
		return 1;
	}

	//Creating address structure
	sockaddr_in serverAddr;
	string serverAddress = "127.0.0.1";
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, serverAddress.c_str(), &(serverAddr.sin_addr));

	//Connecting
	if (connect(s, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Connection failed!" << endl;
		closesocket(s);
		WSACleanup();
		return 1;
	}
	cout << "Successfully connected to server!" << endl;

	thread sender(sendMsg, s);
	thread receive(receiveMsg, s);

	sender.join();
	receive.join();
}
