#include "Client.h"


Client::Client()
{
	ipAddress = "192.168.0.181";
	port = 5050;
	
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Winsock failed to start. Error" << wsResult << std::endl;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Socket not created. Error" << WSAGetLastError() << std::endl;
	}

}
void Client::TCP(){
	// Initialize winsock
	
	char recv_msg[4096];
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Couldn't connect to ship. Error: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
	}
	else {
		std::cout << "ship connected." << std::endl;
	}
	std::string userInput;
	do {
		// send message
		
		//std::cout << "#Engine: ONLINE" << std::endl;
		//getline(std::cin, userInput);

		//std::string input_length = std::to_string(sizeof(userInput));
		//int sendsizeofmsg = send(sock, input_length.c_str(), input_length.size(), 0);
		//int sendResult = send(sock, userInput.c_str(), userInput.size(), 0);

			// wait for a response
		ZeroMemory(recv_msg, 4096);
		int bytesRecieved = recv(sock, recv_msg, 4096, 0);
			
		if (bytesRecieved > 0) {
			command = std::string(recv_msg, 0, bytesRecieved);
		}

		

	} while (command != "!DISCONNECT");

	closesocket(sock);
	WSACleanup();
}


int Client::CreateSocket() {
	// create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Socket not created. Error" << WSAGetLastError() << std::endl;
		return -1;
	}

	
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	return 0;
}

int Client::Connect()
{
	// connect to server

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Couldn't connect to ship. Error: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	return 0;
}
	
int Client::SendRecv(char recv_msg[4096]) {


	// Do while loop to send and recieve data
	char buff[4096];
	std::string userInput;

	do {
		// send message
		std::cout << "#Engine: " << std::endl;
		getline(std::cin, userInput);
		std::string input_length = std::to_string(sizeof(userInput));
		int sendsizeofmsg = send(sock, input_length.c_str(), input_length.size(), 0);
		int sendResult = send(sock, userInput.c_str(), userInput.size(), 0);

		if (sendsizeofmsg != SOCKET_ERROR && sendResult != SOCKET_ERROR)
		{
			// wait for a response
			ZeroMemory(recv_msg, 4096);
			int bytesRecieved = recv(sock, recv_msg, 4096, 0);
			if (bytesRecieved > 0) {
				std::string msg = std::string(recv_msg, 0, bytesRecieved);

				std::cout << "ship: " << recv_msg << std::endl;
			}
		}

	} while (userInput.size() > 0);


	// Shut down winsock
	return 0;
}

Client::~Client() {
	closesocket(sock);
	WSACleanup();
}



