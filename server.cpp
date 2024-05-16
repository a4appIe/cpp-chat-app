#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <thread>
#include <vector>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define PORT 8080


//Init winsock
bool init() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void interact(SOCKET clientSocket, vector<SOCKET> &clients) {
	cout << "Someone connected!" << endl;
	//Receiving data
	char buffer[4096];
	char name[1024];
	while (true) {
		int bytesRcvd = recv(clientSocket, buffer, sizeof(buffer), 0);
		int nameBytes = recv(clientSocket, name, sizeof(name), 0);
		if (bytesRcvd <= 0) {
			cout << "Client disconnected!" << endl;
			break;
		}
		string message(buffer, bytesRcvd);
		string clientName(name, nameBytes);
		cout << "Message sent by " << clientName << ": " << message << endl;
		for (auto client : clients) {
			if (client != clientSocket) {
				send(client, message.c_str(), message.length(), 0);
				send(client, clientName.c_str(), clientName.length(), 0);
			}
		}
	}
	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end()) {
		clients.erase(it);
	}
	closesocket(clientSocket);
}

int main() {
	//Init winsock
	if (!init()) {
		cout << "Winsock init failed! " << endl;
		return 1;
	}
	//Creating listening socket
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		cout << "Socket creation failed!" << endl;
		return 1;
	}
	//Creating address structure (IP/PORT)
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);

	//Converting IP address to binary format
	if (InetPton(AF_INET, _T("0.0.0.0"), &serverAddr.sin_addr) != 1) {
		cout << "Setting address structure failed!" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//Binding the IP
	if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == INVALID_SOCKET) {
		cout << "Binding IP failed!" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//Listening to connections
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Listening failed!" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	cout << "Started listening to port: " << PORT << endl;
	vector<SOCKET> clients;
	while(true){
		//Accepting connections
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			cout << "Invalid socket created!" << endl;
		}
		clients.push_back(clientSocket);
		//Threading
		thread t1(interact, clientSocket, std::ref(clients));
		t1.detach();
	}
	closesocket(listenSocket);
	WSACleanup();
}
