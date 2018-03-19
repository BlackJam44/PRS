#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h> // socket error codes
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <signal.h>

#include "functions.h"

// closing function
void signal_handle(int sig){
	if (sig == SIGINT){
      close(3);
      printf("\nSocket closed\n");
      exit(-1);
	}
}

int main (int argc, char *argv[]) {
  int COMMUNICATION_PORT;

  // structure addr_in creation
  struct sockaddr_in adresse;
  int size = sizeof(struct sockaddr);
  int valid= 1;
  char buffer[RCVSIZE];
  char echo[RCVSIZE];

  // creation of connection socket
  int connect_sock= socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (connect_sock ==  INVALID_SOCKET) {
    perror("cannot create socket udp\n");
    return -1;
  }

  // enables the reuse of the socket
  setsockopt(connect_sock, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  // setting of addr_in structure
  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(CONNECTION_PORT);
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);

  // Link addr structure and the connection socket descriptor
  if (bind(connect_sock, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Connection bind fail\n");
    close(connect_sock);
    return -1;
  }

  // Initialization of connection with client
  CONNECT* init = (CONNECT*)malloc(sizeof(CONNECT));
  init = openClient(connect_sock, (struct sockaddr*) &adresse);
  COMMUNICATION_PORT = atoi(init->port); // Specific port number
	printf("Communication port: %d\n", COMMUNICATION_PORT);

	// creation of communication socket
	int comm_socket= socket(AF_INET, SOCK_DGRAM, 0);
	// structure addr_in communication creation
	struct sockaddr_in adresse2;
	memset((char*)&adresse2, 0, sizeof(adresse2));
  adresse2.sin_family= AF_INET;
  adresse2.sin_port= htons(COMMUNICATION_PORT);
  adresse2.sin_addr.s_addr= htonl(INADDR_ANY);

  setsockopt(comm_socket, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int)); // enable reuse of socket

  // Link addr structure and the communication socket descriptor
  if (bind(comm_socket, (struct sockaddr*) &adresse2, sizeof(struct sockaddr)) == -1) {
    perror("Communication bind fail\n");
    close(comm_socket);
    return -1;
  }

  while (init->result) {
    printf("\nWaiting for data... \n");
    signal(SIGINT, signal_handle);

    // data reception
    if( recvfrom(comm_socket, buffer, RCVSIZE, 0, (struct sockaddr*) &adresse2, &size) == -1 ){
      perror("Error: recvfrom()\n");
      close(comm_socket);
      exit(-1);
    }
    printf("Received: %s",buffer);

    // data echo
    strcpy(echo, buffer);
    if(sendto(comm_socket, echo, RCVSIZE, 0, (struct sockaddr*) &adresse2, size) == -1){
      perror("Error: sendto()\n");
      close(comm_socket);
      exit(-1);
    }
    printf("Echo sent.\n");

		// stop process
    if (strcmp(buffer,"stop\n") == 0) {
      init->result = 0;
			close(comm_socket);
			printf("Connection closed.\n");
    }

    memset(buffer,0,RCVSIZE);
    memset(echo,0,RCVSIZE);
  }

	close(connect_sock);
  return 0;
}
