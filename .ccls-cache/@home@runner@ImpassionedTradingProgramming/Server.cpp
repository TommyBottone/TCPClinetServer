#include "Server.h"

int setupServer() 
{
    // create a socket
    int listening = socket(_SERVER.DOMAIN, _SERVER.TYPE, _SERVER.PROTOCOL);
    if(listening == -1)
    {
      std::cerr << "You gone done screwed up\n";
    }
    // bind a socket to IP / port
    sockaddr_in hint;
    hint.sin_family = _SERVER.DOMAIN;
    hint.sin_port = htons(_SERVER.PORT);
    inet_pton(_SERVER.DOMAIN, _SERVER.LOOP_BACK.c_str(), &hint.sin_addr);  //internet command to point number to a string

    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
      std::cerr << "Unable to bind to IP/Port\n";
      return -2;
    }
    // Mark the socket for listening
    if(listen(listening, SOMAXCONN) == -1)
    {
      std::cerr << "I'm deaf\n";
      return -3;
    }
    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    int clientSocket = accept(listening, (sockaddr*)&hint, &clientSize);
    
    if(clientSocket == -1)
    {
      std::cerr << "Could not get the client connection\n";
      return -4;
    }

    // Close the listening socket
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(serv, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, serv, NI_MAXSERV, 0);

    if(result)
    {
      //Actually connected
      std::cout << "Host("<<host<<") connected on service("<<serv<<")\n";
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
        //clear the buffer
        memset(buffer, 0, _SERVER.MAX_BUFFER);
        //wait for message
        int bytesRecv = recv(clientSocket, buffer, _SERVER.MAX_BUFFER, 0);
        if(bytesRecv == -1)
        {
          std::cerr << "Connection issue with recv call. Quitting\n";
          break;
        }
        if (bytesRecv == 0)
        {
          std::cout <<"Client disconnected\n";
          break;
        }
        //display message
        std::cout << "Received: " << std::string(buffer, 0, bytesRecv ) <<"\n";
        //resend message
        send(clientSocket, buffer,bytesRecv +1, 0);
      }
      // Close the socket
      close(clientSocket);
      
    auto tTest = [](){std::cout<<"print this"<<std::endl;};
    std::async(std::launch::async, tTest);
    //ThreadPool::getInstance()->_threadPool.emplace_back();

  return 0;
}