#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <tuple>

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

struct MESSAGE
{
  MESSAGE(){};
  MESSAGE(const char* m, TYPE s)
  {
    
    memset(msg, 0, 4096);
    strcpy(msg, m);
    sender = s;
  };
  char msg[4096];
  TYPE sender;
};

const size_t MESSAGE_SIZE = sizeof(MESSAGE);

static int clientSocket;
static int listeningSocket;
//Wrapper around send
std::tuple<int, std::string> sendMessage(MESSAGE message, const int &size);
//Wrapper around receive
std::tuple<int, std::string> recvMessage();