#include "librerias.h"
#include "contador.c"
#include "mayusculas.c"
#include "procesar.c"

int main(int argc, char *argv[]){
	int opt;
	char* archivo;
	int flagF = 0;
	int flagH = 0;

        while ((opt = getopt(argc, argv, "f:h")) != -1) {
                switch (opt) {
			case 'f':
				flagF = 1;
				archivo = optarg ;
				break;
                        case 'h':
				flagH =1;
                                break;
                        default:
                                printf("\nWarning: Ingrese un argumento.\n\n");
               }
        }

	if (flagF == 1){procesar(archivo);}
	
	if (flagF == 0 && flagH == 0){procesar(NULL);}

	if(flagH == 1){
		printf("\nAYUDA DE tp3:\nEste progama contará la cantidad de palabras de N caracteres (cant de palabras de un caracter, de dos, de tres, etc) y cambiará las palabras que encuentre en el texto, de un conjunto de palabras reservadas, a mayúsculas, opciones: \n-f [archivo especifico]\n-h [ayuda]\n\n Si no se ingresa un argumento, el programa toma el archivo del input estandar.\n\n");
	}

        return 0;
}
