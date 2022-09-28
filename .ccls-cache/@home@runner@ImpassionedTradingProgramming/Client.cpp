#include "Server.h"

int setupClient()
{
  std::string ipAddr = _SERVER.LOOP_BACK;
  // Create a socket
  int sock = socket(_SERVER.DOMAIN, _SERVER.TYPE, 0);
  if(sock == -1)
  {
    std::cerr<<"Failed to make socket\n";
    return 1;
  }
  // Creat a hint structure for the server connecting to
  sockaddr_in hint;
  hint.sin_family = _SERVER.DOMAIN;
  hint.sin_port = htons(_SERVER.PORT);
  inet_pton(_SERVER.DOMAIN, ipAddr.c_str(), &hint.sin_addr);
  // Connect to the server on the socket
  int connectResult = connect(sock, (sockaddr*)&hint, sizeof(sockaddr_in));
  if(connectResult == -1)
  {
    std::cerr<<"Client connection failed to connect\n";
    return 1;
  }

  char buffer[_SERVER.MAX_BUFFER];
  std::string userInput;

  // Loop:
  
  do
  {
    // Enter text
    std::cout << "Enter text: \n\t>";
    getline(std::cin, userInput);
    // Send to the server
    int sendRes = send(sock, userInput.c_str(), userInput.size()+1, 0);
    if(sendRes == -1)
    {
      std::cerr <<"Could not send to the server!\n";
      continue;
    }
    memset(buffer, 0, _SERVER.MAX_BUFFER);
    // Wait for response
    int bytesReceived = recv(sock, buffer, _SERVER.MAX_BUFFER, 0);
    if(bytesReceived == -1)
    {
      std::cerr << "Error getting the response from the server\r\n";
    }
    else 
    {
      // Display response
      std::cout << "SERVER>" <<std::string(buffer, bytesReceived) << "\r\n";
    }
  }while(true);
  //close the socket
  close(sock);
  
  return 0;
}