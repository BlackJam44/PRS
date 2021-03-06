#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define RCVSIZE 1024
#define CONNECTION_PORT 5000
#define SOCK_TAB [40]

typedef struct elem_init{
  int result;
  char* port;
} CONNECT;


CONNECT* openServer(int socket, struct sockaddr* addr);
CONNECT* openClient(int socket, struct sockaddr* addr);
int createChannel(int no_port);

char* getPort();

#endif
