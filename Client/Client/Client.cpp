#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client.h"
#include <exception>
#include <string>
#include <iostream>

#define MAX_MSG_LEN 100 // maximum length for string to pass throgh

Client::Client()
{
	// we connect to server that uses TCP. thats why SOCK_STREAM & IPPROTO_TCP
	_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_clientSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

}

Client::~Client()
{
	try
	{
		// using destructor to close socket
		closesocket(_clientSocket);
	}
	catch (...) {}
}


void Client::connect(std::string serverIP, int port)
{

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port tto connect trough
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr =  inet_addr(serverIP.c_str());    // IP of the server

	int status = ::connect(_clientSocket, (struct sockaddr*)&sa, sizeof(sa));

	if (status == INVALID_SOCKET)
		throw std::exception("Cant connect to server");
}

void Client::startConversation()
{
	char m[MAX_MSG_LEN];
	int len = recv(_clientSocket, m, MAX_MSG_LEN - 1, 0);
	m[len] = '\0';
	if (std::string(m) != "server connected")
	{
		std::cout << "authentication failed!" << std::endl;
		return;
	}
	while(true)
	{

		std::string s;
		std::cout << ">>> ";
		std::getline(std::cin, s);
		send(_clientSocket, s.c_str(), s.size(), 0);  // last parameter: flag.


		len = recv(_clientSocket, m, MAX_MSG_LEN, 0);
		m[len] = '\0';
		std::cout <<  m << std::endl;
	}
	
}