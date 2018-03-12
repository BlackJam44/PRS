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
  int udp_sock= socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (udp_sock == INVALID_SOCKET) {
    perror("cannot create socket\n");
    return -1;
  }

  // enables the reuse of the socket
  setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  // setting of addr_in structure
  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(CONNECTION_PORT);
  adresse.sin_addr.s_addr= htonl(INADDR_LOOPBACK);

  // Initialization of connection with server
  CONNECT* init = (CONNECT*)malloc(sizeof(CONNECT));
  init = openServer(udp_sock, (struct sockaddr*) &adresse);
  COMMUNICATION_PORT = init->port; // Specific port number

  // Creation of specific socket
  COMM* spec = (COMM*)malloc(sizeof(COMM));
  spec = createChannel(COMMUNICATION_PORT);
  setsockopt(spec->socket, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int)); // enable reuse of socket

  while (init->result) {
    // data sending
    printf("Type in message to send: \n");
    signal(SIGINT, signal_handle);

    fgets(msg, RCVSIZE, stdin);
    if(sendto(udp_sock, msg, RCVSIZE, 0, (struct sockaddr*) &adresse, size) == -1){
      perror("Error: sendto()\n");
      close(udp_sock);
      exit(-1);
    }
    printf("Message sent.\n");

    // data echo
    if(recvfrom(udp_sock, blanmsg, RCVSIZE, 0, (struct sockaddr*) &adresse, &size) == -1 ){
      perror("Error: recvfrom()\n");
      close(udp_sock);
      exit(-1);
    }
    printf("Echo: %s\n",blanmsg);
    memset(blanmsg,0,RCVSIZE);

    /*// stop process
    if (strcmp(msg,"stop\n") == 0) {
      cont= 0;
    }*/
  }

  close(udp_sock);
  return 0;
}
