#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "service.h"

int main(int argc, char **argv)
{
	struct sockaddr_in addr = {};
	struct sockaddr_in cli_addr;
	int sockfd, conn;
	
 	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) { 
		perror("socket()");
		exit(EXIT_FAILURE); 
	}

	/* ADDR. FAMILY */
	addr.sin_family= AF_INET;
	/* PORT */
	addr.sin_port = htons(8080);
	/* ADDRESS */
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	
	if (bind(sockfd, (struct sockaddr *) &addr, sizeof (addr)) < 0) { 
		perror("bind()");
		exit(EXIT_FAILURE); 
	}

	if (listen(sockfd, 5) < 0) { 
		perror("listen()");
		exit(EXIT_FAILURE); 
	}

	//hijo imprima direccion y puerto de los clientes
	listen(sockfd, 5);
	int clilen = sizeof (cli_addr);
	while ((conn = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) > 0) {
		switch (fork()) {
			case 0:
                close(sockfd);
        		char str[INET_ADDRSTRLEN]; 
    			inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);
    			printf("Datos del cliente: \n");
    			printf("IP: %s\n", str);
    			int port = (int) ntohs(cli_addr.sin_port);        
    			printf("Puerto: %d\n", (int) ntohs(cli_addr.sin_port));
                service(conn);
				return 0;

			case -1: // error
				perror("fork()");
				return 1;
		}

		close(conn);
	}

	return 0;	
}

