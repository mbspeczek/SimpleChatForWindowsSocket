#include "ChatServer.h"

ChatServer::ChatServer(int port, std::string address, int family) {
	listenerSocketParams.sin_port = htons(port);
	listenerSocketParams.sin_family = AF_INET;
	listenerSocketParams.sin_addr.S_un.S_addr = INADDR_ANY;
}
int ChatServer::bindSocket() {
	int status = bind(listenerSocket, (sockaddr*)&listenerSocketParams, sizeof(listenerSocketParams));
	return status;
}
bool ChatServer::createListenerSocket(int af, int type, int protocol) {
	listenerSocket = socket(af, type, protocol);
	if (listenerSocket != INVALID_SOCKET)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

SOCKET ChatServer::getListenerSocket() {
	return listenerSocket;
}

int ChatServer::startListening(int backlog) {
	int status = listen(listenerSocket, backlog);
	FD_ZERO(&connectedClientsSet);
	FD_SET(listenerSocket, &connectedClientsSet);
	return status;
}

fd_set& ChatServer::getConnectedClientsSet() {
	return connectedClientsSet;
}

SOCKET ChatServer::addNewClient() {
	SOCKET newClient = accept(listenerSocket, nullptr, nullptr);
	FD_SET(newClient, &connectedClientsSet);
	return newClient;
}

void ChatServer::deleteClient(SOCKET client) {
	FD_CLR(client, &connectedClientsSet);
}
