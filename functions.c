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


// opening connection functions

int openServer(int socket, struct sockaddr* addr){ // exécuté côté client
	int size = sizeof(struct sockaddr);
	char msg[4];
	char ans[10];

	printf("Initializing connection...\n");
	strcpy(msg, "SYN");

	if(sendto(socket, msg, 4, 0, addr, size) == -1){
		perror("Error: SYN\n");
		close(socket);
		exit(-1);
	}

	if(recvfrom(socket, ans, 8, 0, addr, &size) == -1){
		perror("Error: SYN-ACK reception\n");
		close(socket);
		exit(-1);
	}

	if (strcmp(ans,"SYN-ACK") == 0) {
		strcpy(msg, "ACK");
		if(sendto(socket, msg, 4, 0, addr, size) == -1){
			perror("Error: ACK\n");
			close(socket);
			exit(-1);
		}
	}else{
		perror("Error: SYN-ACK message received\n");
		close(socket);
		exit(-1);
	}
	printf("\tConnection initialized.\n");

  return 1;
}


int openClient(int socket, struct sockaddr* addr){ // exécuté côté serveur
	int size = sizeof(struct sockaddr);
	char msg[8];
	char req1[10];
	char req2[10];

	printf("Waiting for connection...\n");
	strcpy(msg, "SYN-ACK");

	if(recvfrom(socket, req1, 10, 0, addr, &size) == -1){
		perror("Error: SYN reception\n");
		close(socket);
		exit(-1);
	}

	if (strcmp(req1,"SYN") == 0) {
		if(sendto(socket, msg, 8, 0, addr, size) == -1){
			perror("Error: SYN-ACK\n");
			close(socket);
			exit(-1);
		}

		if(recvfrom(socket, req2, 10, 0, addr, &size)== -1){
			perror("Error: ACK reception\n");
			close(socket);
			exit(-1);
		}
		if(strcmp(req2,"ACK") != 0){
			perror("Error: ACK message received\n");
			close(socket);
			exit(-1);
		}
	} else{
			perror("Error: SYN message received\n");
			close(socket);
			exit(-1);
	}

	printf("\tConnection initialized.\n");

  return 1;
}
