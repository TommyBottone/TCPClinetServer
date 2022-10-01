#include "Server.h"

int setupServer() 
{
    // create a socket
    int listeningSocket = socket(_SERVER.DOMAIN, _SERVER.TYPE, _SERVER.PROTOCOL);
    if(listeningSocket == -1)
    {
      std::cerr << "You gone done screwed up\n";
    }
    // bind a socket to IP / port
    sockaddr_in hint;
    hint.sin_family = _SERVER.DOMAIN;
    hint.sin_port = htons(_SERVER.PORT);
    inet_pton(_SERVER.DOMAIN, _SERVER.LOOP_BACK.c_str(), &hint.sin_addr);  //internet command to point number to a string

    if(bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
      std::cerr << "Unable to bind to IP/Port\n";
      return -2;
    }
    // Mark the socket for listening
    if(listen(listeningSocket, SOMAXCONN) == -1)
    {
      std::cerr << "I'm deaf\n";
      return -3;
    }
    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    clientSocket = accept(listeningSocket, (sockaddr*)&hint, &clientSize);
    
    if(clientSocket == -1)
    {
      std::cerr << "Could not get the client connection\n";
      return -4;
    }

    // Close the listening socket
    close(listeningSocket);
    memset(host, 0, NI_MAXHOST);
    memset(serv, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, serv, NI_MAXSERV, 0);

    if(result)
    {
      //Actually connected
      std::cout << "Connected!\n";
    }
    else
    {
      inet_ntop(_SERVER.DOMAIN, &client.sin_addr, host, NI_MAXHOST);
      std::cout <<"Host("<<host<<") connected on service("<<htons(client.sin_port)<<")\n";
    }
    // While receiving - display the message

    char buffer[_SERVER.MAX_BUFFER];
    while (true)
    {
      auto bytesRecv = recvMessage(buffer, _SERVER.MAX_BUFFER);
      if(std::get<0>(bytesRecv) <= 0)
      {
        std::cerr << "Connection issue with recv call. Quitting\n";
        break;
      }
      else
      {
        //display message
        std::cout << "Received: " << std::get<1>(bytesRecv) <<"\n";
      }
    }
    // Close the socket
    close(clientSocket);
  return 0;
}

std::tuple<int, std::string> recvMessage(char* buff, int size)
{
  //clear the buffer
    memset(buff, 0, size);
    //wait for message
    int bytesRecv = recv(clientSocket, buff, size, 0);
    if(bytesRecv == -1)
    {
      std::cerr << "Connection issue with recv call. Quitting\n";
      return std::make_tuple(bytesRecv, "");
    }
    if (bytesRecv == 0)
    {
      std::cout <<"Client disconnected\n";
      return std::make_tuple(bytesRecv, "");
    }
    //resend message
    std::string reply = "Server received message: " + std::string(buff);
    send(clientSocket, reply.c_str(), reply.size(), 0);
    return std::make_tuple(bytesRecv, std::string(buff, bytesRecv ));
}