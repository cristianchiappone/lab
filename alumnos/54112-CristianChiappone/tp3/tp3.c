#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "contar.h"
#include "argumentos.h"

int main(int argc, char **argv){
     int file, pid, contador;
     char *ivalue = NULL;
     char *ptr, *delimitador;
     
    ivalue = argumentos(argc, argv);

    file=open(ivalue, O_RDONLY,0777);
    struct stat st1;
    stat(ivalue,&st1);
    printf("Cantidad de bytes del archivo  %lld \n",(long long) st1.st_size);
    char buffer[st1.st_size];

    if(ivalue){
        ptr=mmap(NULL,st1.st_size,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
        read(file, buffer, sizeof buffer);
        for (int i=0 ; i <= st1.st_size ; i++){
             *(ptr+i) = buffer[i]; 
         }     
    }else{
        printf("No tengo nada que leer :( \n");
    }

    printf("Padre - PID(%d) \n", getpid());

    for(int i = 0; i < 2; i++) {
        switch(fork()) {
            case 0:
                contador=0;
                delimitador=" \n\t";
                contador = contar(st1.st_size,ptr,delimitador);
                printf("Hijo%d - PID(%d). Cdad palabras contadas: %d\n", i,getpid(),contador);
                return 0;
            case -1:
                perror("fork()");
                return -1;
        }
    }

    
    if ((pid = fork() == 0)){
        printf("Hijo3 - PID(%d). Historial de palabras contadas: \n",getpid());
        char *palabras[] = {"de", "ONU", "deben"};
        int cdad_palabras = (sizeof(palabras)/sizeof(palabras[0]));
        for (int i = 0; i < cdad_palabras; i++) {
            int cdad_total_x_palabra = 0;
            char *texto = ptr;
            while ((texto=strstr(texto,palabras[i]))!= NULL ) {
                texto += strlen(palabras[i]);
                cdad_total_x_palabra++;
            }
            printf("La palabra #%s, se encontró %u veces.\n", palabras[i],cdad_total_x_palabra);

        }
    }
    close(file);
    exit(0);
}

