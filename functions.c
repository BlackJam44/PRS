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
#include <time.h>

#include "functions.h"


// opening connection functions
CONNECT* openServer(int socket, struct sockaddr* addr){ // exécuté côté client
	int size = sizeof(struct sockaddr);
	char msg[4];
	char ans[10];
	char* no_port = (char*)malloc(sizeof(char));

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

	if(recvfrom(socket, no_port, 5, 0, addr, &size) == -1){
		perror("Error: port number reception\n");
		close(socket);
		exit(-1);
	} else printf("Port number received: %s\n", no_port);

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

	// returned structure
	CONNECT* res = (CONNECT*)malloc(sizeof(CONNECT));
	res->result = 1;
	res->port = (char*)malloc(sizeof(char));
	strcpy(res->port, no_port);

  return res;
}

CONNECT* openClient(int socket, struct sockaddr* addr){ // exécuté côté serveur
	int size = sizeof(struct sockaddr);
	char msg[8];
	char req1[10];
	char req2[10];
	char* no_port = (char*)malloc(sizeof(char));

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

		// wait(1);
		strcpy(no_port, getPort());
		printf("Sending port number %s\n", no_port);
		if(sendto(socket, no_port, 5, 0, addr, size) == -1){
			perror("Error: port number\n");
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

	// returned structure
	CONNECT* res = (CONNECT*)malloc(sizeof(CONNECT));
	res->result = 1;
	res->port = (char*)malloc(sizeof(char));
	strcpy(res->port, no_port);

	return res;
}


// génère un numéro de port random entre 5001 et 10000
char* getPort(){
	srand(time(NULL));
	char* res = (char*)malloc(sizeof(char));
	int a = (rand()%5000) + 5001;
	snprintf(res, 5, "%d", a);
	printf("random port generated : %s\n", res);
	return res;
}
