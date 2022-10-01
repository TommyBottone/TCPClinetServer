#include "Server.h"

int setupClient()
{
  std::string ipAddr = _SERVER.LOOP_BACK;
  // Create a socket
  clientSocket = socket(_SERVER.DOMAIN, _SERVER.TYPE, 0);
  if(clientSocket == -1)
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
  int connectResult = connect(clientSocket, (sockaddr*)&hint, sizeof(sockaddr_in));
  if(connectResult == -1)
  {
    std::cerr<<"Client connection failed to connect\n";
    return 1;
  }

  std::string userInput;

  // Loop:  
  do
  {
    // Enter text
    std::cout << "Enter text: \n\t>";
    getline(std::cin, userInput);
    
    MESSAGE msg(userInput.c_str(), TYPE::CLIENT);
    
    // Send to the server
    auto returnVal = sendMessage(msg, MESSAGE_SIZE);
    if(std::get<0>(returnVal) >= 0)
    {
      std::cout << "Repeat from SERVER: " << std::get<1>(returnVal) << "\r\n";
    }
    
  }while(true);
  //close the socket
  close(clientSocket);
  
  return 0;
}

std::tuple<int, std::string> sendMessage(MESSAGE message, const int &size)
{
  int sendRes = send(clientSocket, &message, size, 0);
  if(sendRes == -1)
  {
    std::cerr <<"Could not send to the server!\n";
    return std::make_tuple(-1, "");
  }
  // Wait for response
  int bytesReceived = recv(clientSocket, &message, MESSAGE_SIZE, 0);
  if(bytesReceived == -1)
  {
    std::cerr << "Error getting the response from the server\r\n";
    return std::make_tuple(-2, "");
  }
  if(message.sender != TYPE::SERVER)
  {
    std::cerr <<"Message not sent from the server\r\n";
    return std::make_tuple(-3, "");
  }
  return std::make_tuple(bytesReceived, std::string(message.msg, bytesReceived));
}