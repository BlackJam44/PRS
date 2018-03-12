#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define RCVSIZE 1024
#define CONNECTION_PORT 5000
#define SOCK_TAB [40]

typedef struct elem_init{
  int result;
  char port[5];
} CONNECT;

typedef struct elem_com{
  int result;
  int socket;
  struct addr* comm_addr;
} COMM;

CONNECT* openServer(int socket, struct sockaddr* addr);
CONNECT* openClient(int socket, struct sockaddr* addr);
COMM* createChannel(char no_port[5]);

char* getPort();

#endif
