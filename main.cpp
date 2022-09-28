#include "Server.h"
#include <iostream>

extern int setupServer();
extern int setupClient();

/*
TCP Client-Server
Pass in argument client to create a client
Pass in server to create a server
*/

int main(int argc, char** argv) 
{
  TYPE type = TYPE::SERVER;

  if(argc > 1)
  {
    if(strcmp(argv[argc-1], "client") == 0)
    {
      type = TYPE::CLIENT;
    }
    else if(strcmp(argv[argc-1], "server") == 0)
    {
      type = TYPE::SERVER;
    }
    else
    {
      std::cerr<< "Pass in either client or server"<<std::endl;
      return 0;
    }
  }
  else
  {
      std::cerr<< "Pass in either client or server"<<std::endl;
      return 0;
  }
  if(TYPE::SERVER == type)
  {
    std::cout <<"SERVER!"<<std::endl;
    
    if(setupServer() != 0)
    {
      std::cerr <<"There was an error with the server.\n";
      std::string s;
      std::cin>>s;
    }
  }
  else 
  {
    std::cout <<"CLIENT!"<<std::endl;
    if(setupClient() != 0)
    {
      std::cerr <<"There was an error with the client.\n";
      std::string s;
      std::cin>>s;
    }
  }
  return 0;
  
}