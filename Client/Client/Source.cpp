
#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include "WSAInitializer.h"
#include "Client.h"
#include <exception>
#include <iostream>

using namespace std;

int main()
{
	try
	{
		WSAInitializer wsaInit;
		Client c1;
		c1.connect("127.0.0.1", 2357);
		c1.startConversation();

	}
	catch (exception& e)
	{
		cout << "Error occured: " << e.what() << endl;
	}
	system("PAUSE");
	return 0;
}