#include <iostream>
#include <sstream>
#include<WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#include "ChatServer.h"

int main() {

	WSADATA wsData;

	int wsOk = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (wsOk != 0) {
		std::cout << "Cant start" << std::endl;
		WSACleanup();
		return 1;
	}

	ChatServer server(54000, " ", 0);

	if (server.createListenerSocket(AF_INET, SOCK_STREAM, 0) != 0) {
		std::cout << "cant create listener socket";
		WSACleanup();
		return 1;
	}

	if (int status = server.bindSocket() != 0) {
		std::cout << "cant bind socket "<< status<<" "<< WSAGetLastError();
		WSACleanup();
		return 1;
	}

	server.startListening(10);

	while (true) {
		fd_set tmpSet = server.getConnectedClientsSet();
		int socketCount = select(0, &tmpSet, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET tmpSocket = tmpSet.fd_array[i];
			
			if (tmpSocket == server.getListenerSocket()) {
				
				auto newClient = server.addNewClient();
				
				std::string helloMessage = "Hello \r\n";
				send(newClient, helloMessage.c_str(), helloMessage.size() + 1, 0);
			}
			else
			{
				char buffer[512];
				ZeroMemory(buffer, 512);

				int recivedBytes = recv(tmpSocket, buffer, sizeof(buffer), 0);
				if (recivedBytes <= 0)
				{
					closesocket(tmpSocket);
					server.deleteClient(tmpSocket);
				}
				else
				{
					/* broadcast message */
					for (int i = 0; i < server.getConnectedClientsSet().fd_count; i++) {
						SOCKET receiver = server.getConnectedClientsSet().fd_array[i];
						if (receiver != server.getListenerSocket() && receiver != tmpSocket) {
							std::ostringstream stream;
							stream << "FROM " << tmpSocket << ": " << buffer<<"\r\n";
							std::string outputString = stream.str();
							
							send(receiver, outputString.c_str(), outputString.size() + 1, 0);
						}
					}
				}
			}
		}
	}


	WSACleanup();

	return 0;
}