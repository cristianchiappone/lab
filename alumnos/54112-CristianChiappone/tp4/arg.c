#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "arg.h"

int get_opt(int argc, char **argv){
    int pvalue;

     int opt;
     opterr = 0;
 
     while ((opt = getopt (argc, argv, "p:")) != -1) {
         switch(opt)
         {
             case 'p':
                 pvalue = atoi(optarg);
                 break;
 
             case '?':
                 if (optopt == 'p')
                     fprintf (stderr, "La opcion -%o requiere un argumento\n", optopt);
                 else if (isprint (optopt))//checkea que un caracter pueda ser impreso
                     fprintf (stderr, "Opcion desconocida -%c.\n", optopt);
                 break;
 
             default:
                 abort ();
         }
     }
      return pvalue;
}