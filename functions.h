#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define RCVSIZE 1024
#define PORT_SERV 5001
#define SOCK_TAB [40]

int openServer(int socket, struct sockaddr* addr);
int openClient(int socket, struct sockaddr* addr);

#endif
