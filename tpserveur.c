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

  // structure addr_in creation
  struct sockaddr_in adresse;
  int size = sizeof(struct sockaddr);
  int valid= 1;
  char buffer[RCVSIZE];
  char echo[RCVSIZE];

  // create socket udp
  int udp_sock= socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (udp_sock ==  INVALID_SOCKET) {
    perror("cannot create socket udp\n");
    return -1;
  }

  // enables the reuse of the socket
  setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  // setting of addr_in structure
  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(PORT_SERV);
  adresse.sin_addr.s_addr= htonl(INADDR_ANY);

  // Link addr structure and the socket descriptor
  if (bind(udp_sock, (struct sockaddr*) &adresse, sizeof(adresse)) == -1) {
    perror("Bind fail\n");
    close(udp_sock);
    return -1;
  }


  int cont = openClient(udp_sock, (struct sockaddr*) &adresse) ;

  while (cont) {
    printf("\nWaiting for data... \n");
    signal(SIGINT, signal_handle);

    // data reception
    if( recvfrom(udp_sock, buffer, RCVSIZE, 0, (struct sockaddr*) &adresse, &size) == -1 ){
      perror("Error: recvfrom()\n");
      close(udp_sock);
      exit(-1);
    }
    printf("Received: %s",buffer);

    // data echo
    strcpy(echo, buffer);
    if(sendto(udp_sock, echo, RCVSIZE, 0, (struct sockaddr*) &adresse, size) == -1){
      perror("Error: sendto()\n");
      close(udp_sock);
      exit(-1);
    }
    printf("Echo sent.\n");
    memset(buffer,0,RCVSIZE);
    memset(echo,0,RCVSIZE);
  }


  return 0;
}
