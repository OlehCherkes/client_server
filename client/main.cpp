#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main() {
  // initialization of socket library
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "Failed to initialize Winsock" << std::endl;
    return 1;
  }
  // creates socket, AF_INET - IPv4, SOCK_STREAM - TCP
  SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == INVALID_SOCKET) {
    std::cerr << "Failed to create socket" << std::endl;
    WSACleanup();
    return 1;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345);
  inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));

  // establishing connections to the server
  if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    std::cerr << "Connect failed with error" << WSAGetLastError() << std::endl;
    closesocket(clientSocket);
    WSACleanup();
    return 1;
  }

  // send mesage
  const char* message = "Hello from client!";
  if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
    std::cerr << "Send failed with error" << WSAGetLastError() << std::endl;
  }
  else {
    std::cout << "Message sent successfully" << std::endl;
  }

  closesocket(clientSocket);
  WSACleanup();

  return 0;
}