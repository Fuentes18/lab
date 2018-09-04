#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int unirDefault();
int unir(char*);

int main(int argc, char * argv[]){
	char opt;
	int flag = 0;
	while ((opt = getopt(argc, argv, "o:h")) != -1 ) { 
         	switch (opt) {
			case 'o': 
				flag = 1;
                                unir(optarg);  
                                break;
	  		case 'h':
				flag = 1;
				printf("\nAYUDA DE PEGAR:\nSirve para pegar X cantidad de archivos\n-o [especifica el nombre del archivo resultante]\n\n");
				break;
			default: 
				flag = 1;
				printf ("\nWarning :Ingrese un argumento.\n");
				break;
             	}
        }
	if (flag == 0) {
		unirDefault();	
	}  
        return 0;
}
int unir(char *nombre) {

       int destino , leido, fdd;
       int x = 0;
       int j = 0;
       char buffer[200];
       char* archivos[100];
       char* trozo = NULL;

       destino = open(nombre, O_RDWR|O_CREAT, 0666); 
       while ( ( leido = read ( STDIN_FILENO, &buffer, sizeof buffer ) ) > 0) {   
         	 
       	trozo = strtok (buffer,"\n");  
       	while (trozo != NULL){
        	archivos[x]=trozo;
               	trozo = strtok (NULL, "\n");
              	x++;
       	}
       }

       for(j=0;j<x; j++ ){
	fdd = open (archivos[j],O_RDONLY, 0666);
	while ( ( leido = read (fdd, &buffer, sizeof buffer ) ) > 0) {   
                write (destino, buffer, leido); 
       	}
	close(fdd);
       }

	 
       close(destino);
	
       return 0;
}

int unirDefault() { 
	int leido;
        char buffer[50];

        while ( ( leido = read ( STDIN_FILENO, &buffer, sizeof buffer ) ) > 0) {
                write (STDOUT_FILENO, buffer, leido);
        }
        
        return 0;
}
