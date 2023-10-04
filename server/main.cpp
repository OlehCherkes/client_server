#include <iostream>
#include <winsock2.h>

int main() {
  // initialization of socket library
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { 
    std::cerr << "Failed to initialize Winsock" << std::endl;
    return 1;
  }

  // creates socket, AF_INET - IPv4, SOCK_STREAM - TCP
  SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == INVALID_SOCKET) {
    std::cerr << "Failed to create socket" << std::endl;
    WSACleanup();
    return 1;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;  // IPv4
  serverAddr.sin_addr.s_addr = INADDR_ANY; // connections from any IP addresses.
  serverAddr.sin_port = htons(12345); // server port

  // binding a socket to a specific address and port
  if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    std::cerr << "Bind failed with error" << WSAGetLastError() << std::endl;
    closesocket(serverSocket);
    WSACleanup();
    return 1;
  }

  // setting the socket to listening mode
  if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
    std::cerr << "Listen failed with error" << WSAGetLastError() << std::endl;
    closesocket(serverSocket);
    WSACleanup();
    return 1;
  }

  std::cout << "Server listening on port 12345..." << std::endl;

  SOCKET clientSocket = accept(serverSocket, NULL, NULL);
  if (clientSocket == INVALID_SOCKET) {
    std::cerr << "Accept failed with error" << WSAGetLastError() << std::endl;
    closesocket(serverSocket);
    WSACleanup();
    return 1;
  }

  char buffer[1024];
  int bytesReceived;

  do {
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0); // > 0 OK, < 0 error, == 0connection closet by client
    if (bytesReceived > 0) {
      buffer[bytesReceived] = '\0';
      std::cout << "Received: " << buffer << std::endl;
    }
    else if (bytesReceived == 0) {
      std::cout << "Client disconnected" << std::endl;
    }
    else {
      std::cerr << "Receive failed with error" << WSAGetLastError() << std::endl;
    }
  } while (bytesReceived > 0);

  closesocket(clientSocket);
  closesocket(serverSocket);
  WSACleanup();

  return 0;
}