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
#include <pthread.h>	

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	struct sockaddr_in direccion = {};
	int sockfd, sd_conn, *sdc;
	int val = 1;
	
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

 	pthread_t tid;
	
	if (bind(sockfd, (struct sockaddr *) &direccion , sizeof (direccion)) < 0) { 
		perror("bind()");
		exit(EXIT_FAILURE); 
	}

	if (listen(sockfd, 5) < 0) { 
		perror("listen()");
		exit(EXIT_FAILURE); 
	}
//	signal(SIGCHLD,SIG_IGN); //ignora como termino el hijo, x lo tanto no crea zombies

	while ( (sd_conn = accept(sockfd, NULL, 0)) > 0) {
	if((sdc=malloc(sizeof (int))) == NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
   *sdc=sd_conn;
      if ((pthread_create(&tid,NULL,&service,(void *)sdc))!=0){
            perror("pthread");
				exit(EXIT_FAILURE); 
      }
	}
   pthread_exit(NULL);
	return 0;	
}

