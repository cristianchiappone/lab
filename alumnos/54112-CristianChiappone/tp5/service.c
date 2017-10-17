#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* inet_aton */
#include <netdb.h>
#include "service.h"
#include "pthread.h"

void *service(void *sd_conn) {
	char buff[1024], copy[1024], method[4], resource[100], filename[50], extension[7];
	int leido,fd, sdc;
	char *last_token;
	char *jpg_header = "HTTP/1.0 200 Ok\r\nContent-Type: image/jpeg\r\n\r\n";
	char *png_header = "HTTP/1.0 200 Ok\r\nContent-Type: image/png\r\n\r\n";
	char *pdf_header = "HTTP/1.0 200 Ok\r\nContent-Type: application/pdf\r\n\r\n";
	char *html_header = "HTTP/1.0 200 Ok\r\nContent-Type: text/html\r\n\r\n";
	char *gif_header = "HTTP/1.0 200 Ok\r\nContent-Type: image/gif\r\n\r\n";
	char *textplain_header = "HTTP/1.0 200 Ok\r\nContent-Type: text/plain\r\n\r\n";
	char *err_404 = "HTTP/1.1 404 NOT FOUND\nContent-type: text/html\nContent-Length: 13\n\n404 NOT FOUND";
	char *err_403 =  "HTTP/1.1 403 FORBIDDEN\nContent-type: text/html\nContent-Length: 13\n\n403 FORBIDDEN"; 
	char *err_405 = "HTTP/1.1 405 METHOD NOT ALLOWED\nContent-type: text/html\nContent-Length: 23\n\n405 METHOD NOT ALLOWED"; 
	char *err_500 = "HTTP/1.1 500 INTERNAL SERVER ERROR\nContent-type: text/html\nContent-Length: 25\n\n500 INTERNAL SERVER ERROR"; 
    sdc = *((int *)sd_conn);
    if(sd_conn != NULL){
    	free(sd_conn);
    }
    	
    leido = read(sdc, buff, sizeof buff);

	printf("El cliente me escribio %s \n", buff);

	strcpy(copy, buff); //hago una copia del buffer para poder usar strtok
   	
   	last_token = strtok(copy, " "); //busco un espacio en copia, cuando lo encuentra me devuelve un puntero al ultimo token
	strcpy(method, last_token);
   	last_token = strtok(NULL, " ");
	strcpy(resource, last_token+1);

	if ((strcmp(buff, "") == 0)){
		write(sdc, err_500, strlen(err_500));
	}else if (!(strncmp(method, "GET",3) == 0)){
		write(sdc, err_405, strlen(err_405));
	}else if ((fd = open(resource, O_RDONLY)) < 0){ 
		switch(errno){
			case 13:	
				write(sdc, err_403, strlen(err_403));
				exit(EXIT_FAILURE); 
				break;
			case 2:
				write(sdc, err_404, strlen(err_404));
				exit(EXIT_FAILURE); 
				break;
		}
		close(fd);
	}else{

		strcpy(filename, resource);
		last_token = strtok(filename, ".");
		last_token = strtok(NULL, ".");
		strcpy(extension, last_token);

		if(strcmp(extension, "jpg")==0){ // == 0 si ambas cadenas son identicas
			write(sdc, jpg_header, strlen(jpg_header));
		}else if(strcmp(extension, "png")==0){
			write(sdc, png_header, strlen(png_header));
		}else if(strcmp(extension, "pdf")==0){
			write(sdc, pdf_header, strlen(pdf_header));
		}else if(strcmp(extension, "html")==0){
			write(sdc, html_header, strlen(html_header));
		}else if(strcmp(extension, "gif")==0){
			write(sdc, gif_header, strlen(gif_header));
		}else{
			write(sdc, textplain_header, strlen(textplain_header));
		}

		while((leido = read(fd, copy, sizeof copy)) != 0){
			write(sdc, copy, leido);
		}
		close(fd);
	}
	close(sdc);
	pthread_exit(NULL);
	//return 0;
}
