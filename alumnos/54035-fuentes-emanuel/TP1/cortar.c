#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int cortar(int, int, char*, char*);
int cortarDefault();

int main(int argc, char *argv[]){
        char* nombre;
	char* archivo;
        int opt, cantidad, medida;
        int flagOpt=0;
	int flagN=0;
	int flagS = 0;
	int flagH = 0;
	int flagI = 0;

        while ((opt = getopt(argc, argv, "n:s:o:i:h")) != -1) {
                switch (opt) {
			case 'n':
				flagN = 1;
				cantidad = atoi(optarg);
				break;
			case 's':
				flagS = 1;
				medida = atoi(optarg);
				break;
                        case 'o':
                                flagOpt = 1;
				nombre = optarg;
                                break;
			case 'i':
				flagI = 1;
				flagOpt = 1;
				archivo = optarg;
				break;
                        case 'h':
				flagH =1;
                                flagOpt = 1;
                                break;
                        default:
                                flagOpt = 1;
                                printf("\nWarning: Ingrese un argumento.\n\n");
               }
        }
	
	if (flagI == 0){archivo = NULL;}

	if (flagN == 1 && flagS == 0){cortar (1, cantidad, nombre,archivo);}
	
	if (flagS == 1 && flagN == 0){cortar(2, medida, nombre,archivo);}
	
	if (flagS == 1 && flagN == 1){printf("\n\nWrong: No se pueden utilizar la opcion n y s, son excluyentes.\n\n");}


        if (flagOpt == 0){
                cortarDefault();
        }

	if (flagH == 1){
		printf("\nAYUDA DE CORTAR:\nEste programa sirve para cortar un archivo en X cantidad de archivos\n-n [especifica la cantidad de archivos resultantes]\n-s [especifica el tamaño para de los archivos cortados]\n-o [nombre del archivo cortado]\n\n");
	}

        return 0;
}

int cortar(int opcion, int parametro, char* nombre_archivo, char* archivo){
	
	char nombre[25];
	char numero[5];
	char *base;
	int fd;
	

	base = "ArchivoCortado";

	if (archivo != NULL){
		fd = open(archivo,O_RDWR|O_CREAT,0666);
	}else{
		fd = STDIN_FILENO;
	}

	if(opcion == 1){
		        
			int destino,leido,auxiliar;
			char buffer[50];
			int i = 1;
			int aux = 1;
			int limite;
			
			//CREO UN ARCHIVO TEMPORAL QUE LUEGO ELIMINO
			auxiliar = open ("temporal",O_RDWR|O_CREAT,0666);
			while((leido = read(fd,&buffer, sizeof buffer)) >0){
				aux ++;	
				write (auxiliar,buffer,leido);				
			}			
			close(auxiliar);
		//	
			//COMPRUEBO LA CANTIDAD DE BUFFERS POR ARCHIVO, SI HAY UN RESTO LE SUMO 1
			if(aux % parametro != 0){aux = (aux/parametro)+1;
			}else{aux = aux/parametro;}
			
			//SABIENDO COMO LO VOY A DIVIDIR EMPIEZO A INGRESARLO EN LOS ARCHIVOS
			limite = aux;
			auxiliar = open ("temporal",O_RDONLY);
                        while((leido = read(auxiliar,&buffer, sizeof buffer)) >0){ 
				if (limite == aux ){
					close(destino);

					//SECCION DEL NOMBRE
					if (nombre_archivo != NULL){
                				sprintf(nombre, "%s", nombre_archivo);
        				}else{
                				sprintf(nombre, "%s", base);
        				}
					sprintf(numero, "%d", i);
					strcat(nombre,numero);
				
					//CIERRE SECCION NOMBRE

                               	 	destino = open(nombre, O_RDWR|O_CREAT, 0666);
					limite = 0;
					i++;
                                }
				write(destino,buffer,leido);
				limite++;
                        }
			close(auxiliar);
			remove("temporal");
        }

	if(opcion == 2){
                        int destino,leido;
                        char buffer[parametro];
                        int i = 1;
			
			while((leido = read(fd,&buffer, sizeof buffer)) >0){
        			
				//SECCION DEL NOMBRE
                                if (nombre_archivo != NULL){
                                        sprintf(nombre, "%s", nombre_archivo);
                                }else{
                                        sprintf(nombre, "%s", base);
                                }
                                sprintf(numero, "%d", i);
                                strcat(nombre,numero);

                                //CIERRE SECCION NOMBRE                       
	
	                        destino = open(nombre, O_RDWR|O_CREAT, 0666);
                                i++;
                                write(destino,buffer,leido);
                                close(destino);
                        }


        }

	return 0;
}

int cortarDefault(){
	int leido;
        char buffer[50];

        while((leido = read(STDIN_FILENO,&buffer, sizeof buffer)) >0){
                write(STDOUT_FILENO,buffer,leido);
        }
        return 0;
}
