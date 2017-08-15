#ifndef _argumentos_h_
#define _argumentos_h

char* argumentos(int argc, char **argv){

     int iflag = 0;
     char *ivalue = NULL;
     int opt;
 
     opterr = 0;
 
     while ((opt = getopt (argc, argv, "i:")) != -1) {
         switch(opt)
         {
             case 'i':
                 iflag = 1;
                 ivalue = optarg;
                 break;
 
             case '?':
                 if (optopt == 'i')
                     fprintf (stderr, "La opcion -%o requiere un argumento\n", optopt);
                 else if (isprint (optopt))//checkea que un caracter pueda ser impreso
                     fprintf (stderr, "Opcion desconocida -%c.\n", optopt);
                 break;
 
             default:
                 abort ();
         }
     }
      printf("iflag = %d, ivalue = %s\n", iflag, ivalue);
      return ivalue;
     //Fin de lectura de argumentos
}
#endif 
