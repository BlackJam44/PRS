#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>  // socket error codes
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

  // structure addr_in creation
  struct sockaddr_in adresse;
  memset((char*)&adresse, 0, sizeof(adresse));
  int size = sizeof(struct sockaddr);
  int valid= 1;
  int COMMUNICATION_PORT;
  char msg[RCVSIZE];
  char blanmsg[RCVSIZE];

  // create socket udp
  int connect_sock= socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (connect_sock == INVALID_SOCKET) {
    perror("cannot create socket\n");
    return -1;
  }

  // enables the reuse of the socket
  setsockopt(connect_sock, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  // setting of addr_in structure
  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(CONNECTION_PORT);
  adresse.sin_addr.s_addr= htonl(INADDR_LOOPBACK);

  // Initialization of connection with server
  CONNECT* init = (CONNECT*)malloc(sizeof(CONNECT));
  init = openServer(connect_sock, (struct sockaddr*) &adresse);
  COMMUNICATION_PORT = atoi(init->port); // Specific port number
	printf("Communication port: %d\n", COMMUNICATION_PORT);

	// create socket udp
	int comm_socket= socket(AF_INET, SOCK_DGRAM, 0);

	// handle error
	if (comm_socket == INVALID_SOCKET) {
		perror("cannot create socket\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in adresse2;
	memset((char*)&adresse2, 0, sizeof(adresse2));
  adresse2.sin_family= AF_INET;
  adresse2.sin_port= htons(COMMUNICATION_PORT);
  adresse2.sin_addr.s_addr= htonl(INADDR_LOOPBACK);

	printf("Channel created.\n");
  setsockopt(comm_socket, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int)); // enable reuse of socket

  while (init->result) {
    // data sending
    printf("Type in message to send: \n");
    signal(SIGINT, signal_handle);

    fgets(msg, RCVSIZE, stdin);
    if(sendto(comm_socket, msg, RCVSIZE, 0, (struct sockaddr*) &adresse2, size) == -1){
      perror("Error: sendto()\n");
      close(comm_socket);
      exit(-1);
    }

    printf("Message sent.\n");

    // data echo
    if(recvfrom(comm_socket, blanmsg, RCVSIZE, 0, (struct sockaddr*) &adresse2, &size) == -1 ){
      perror("Error: recvfrom()\n");
      close(comm_socket);
      exit(-1);
    }
    printf("Echo: %s\n",blanmsg);
    memset(blanmsg,0,RCVSIZE);

		// stop process
		if (strcmp(msg,"stop\n") == 0) {
			init->result = 0;
			close(comm_socket);
			printf("Connection closed.\n");
		}

  }

  close(connect_sock);
  return 0;
}
