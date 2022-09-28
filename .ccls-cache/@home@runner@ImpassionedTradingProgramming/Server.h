#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

const struct Server 
{
  const int DOMAIN = AF_INET;  //ipv4
  const int TYPE = SOCK_STREAM;
  const int PROTOCOL = 0;
  const int PORT = 54000;
  const std::string ANY = "0.0.0.0";
  const int MAX_BUFFER = 4096;
  const std::string LOOP_BACK = "127.0.0.1";
} _SERVER;

enum TYPE
{
  SERVER = 0,
  CLIENT = 1
};
