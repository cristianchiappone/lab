#ifndef _contar_h_
#define _contar_h

int contar(int cantidad,char* ptr, char* delimitador){
        int contador=0;
        char *cadena,copia[cantidad];
        strcpy(copia,ptr);
        if(cantidad!=80){
	        cadena = strtok(copia,delimitador);
	        while(cadena != NULL){
	                cadena=strtok(NULL,delimitador);
	                contador++;
	        }
	        return contador;
	    }
}
#endif 

