#pragma once
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

class ChatServer {
public:
	ChatServer(int port, std::string address, int family);
	
	bool createListenerSocket(int af, int type, int protocol);
	int bindSocket();	
	int startListening(int backlog);
	SOCKET getListenerSocket();

	fd_set& getConnectedClientsSet();
	SOCKET addNewClient();
	void deleteClient(SOCKET client);

private:
	SOCKET listenerSocket;
	sockaddr_in listenerSocketParams;
	fd_set connectedClientsSet;
};