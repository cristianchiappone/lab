#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* inet_aton */
#include <netdb.h>
#include <string.h>
#include "service.h"
#include "arg.h"

int main(int argc, char **argv)
{
	struct sockaddr_in direccion = {};
	int sockfd, conn;
	int val;
	
 	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) { 
		perror("socket()");
		exit(EXIT_FAILURE); 
	}

	/* ADDR. FAMILY */
	direccion.sin_family= AF_INET;
	/* PORT */
	int port = get_opt(argc, argv);
	direccion.sin_port = htons(port);
	/* ADDRESS */
	inet_pton(AF_INET, "127.0.0.1", &direccion.sin_addr);
	setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR,&val,sizeof(val));
	
	if (bind(sockfd, (struct sockaddr *) &direccion , sizeof (direccion)) < 0) { 
		perror("bind()");
		exit(EXIT_FAILURE); 
	}

	if (listen(sockfd, 5) < 0) { 
		perror("listen()");
		exit(EXIT_FAILURE); 
	}
	signal(SIGCHLD,SIG_IGN); //ignora como termino el hijo, x lo tanto no crea zombies

	while ( (conn = accept(sockfd, NULL, 0)) > 0) {
		switch (fork()) {
			case 0: 	// hijo
				service(conn);
				return 0;
			case -1:	// error
				perror("fork()");
				return 1;
		}
		close(conn);
	}
	
	return 0;	
}

