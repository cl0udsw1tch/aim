#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#include <Windows.h>
#pragma comment (lib, "ws2_32.lib")

class Client {
private:
	std::string ipAddress;
	int port;
	sockaddr_in hint;
	SOCKET sock;
	

public:
	std::string command = "";
	std::string prev_command = "";
	Client();
	~Client();
	void TCP();
	int CreateSocket();
	int Connect();

	int SendRecv(char recv_msg[4096]);

};