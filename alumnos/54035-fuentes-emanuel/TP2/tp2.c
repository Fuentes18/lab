#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

char* mayusculas(char*, int, int, char*);
int procesar (char*);
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
		printf("\nAYUDA DE tp2:\nEste programa sirve para procesar un archivo y obtener palabras, caracteres del mismo, opciones: \n-f [archivo especifico]\n-h [ayuda]\n\n Si no se ingresa un argumento, el progama toma el archivo del input estandar\n\n");
	}

        return 0;
}

int procesar(char* archivo){
	
	int leido,fd;
	int x = 0;
	int aux = 0;
	int pd_padre[2], pd_hijo[2];
	char buffer[50];
	char buffer_punto[1024];
	char* conversion;
	int pid;
	int pid2;

	if (archivo != NULL){ fd = open (archivo,O_RDONLY);
	}else{ fd = STDIN_FILENO;}
	
	pipe(pd_padre);
	pipe(pd_hijo);

	pid = fork();
	if (pid){pid2 = fork();} //Correccion 12-6-18, El padre crea el nuevo hilo, sin el if, el hijo tambien creaba un proceso
 
	if(pid == 0){ //CONVIERTE A MAYUSCULAS
		close(pd_hijo[0]);

		while((leido = read(pd_padre[0],&buffer_punto, sizeof buffer_punto)) > 0 ){
			conversion = mayusculas(buffer_punto,leido,9,"Argentina"); 
			write (pd_hijo[1],conversion,leido);
			
		}

		return 0;	
	}
		

	if(pid2 == 0){ //LEE LAS PALABRAS DEL BUFFER
		close(pd_hijo[1]);
		
		while((leido = read(pd_hijo[0],&buffer_punto, sizeof buffer_punto)) > 0 ){
			write(STDOUT_FILENO,buffer_punto,leido);
		        printf("\nSe leyó %d palabras de %d caracteres.\n",contar(buffer_punto,leido),leido);	
      		}

                return 0;       
        }
	
	close(pd_padre[0]);
	while((leido = read(fd,&buffer, sizeof buffer)) > 0 ){ //PADRE LEE Y COLOCA EN EL BUFFER

		for(x=0;x<leido;x++){
                        buffer_punto[aux] = buffer[x];

			if (buffer_punto[aux] == '.'){
				write (pd_padre[1], &buffer_punto,(aux+1));
				aux = 0;	
			}else{
				aux++;
			}
                }
		sleep(1);
        }
	close(pd_padre[1]);
	wait(NULL);
	return 0;	
}



char* mayusculas(char* buf, int leido, int caracteres, char *palabra){
	int x, i, aux, aux2;
	int o =0;
	int z = 0;
	
	char cambiar[caracteres];
	strcpy(cambiar, palabra);


	for(x=0;x<leido;x++){
		z++;
        	if((buf[x] == ' '|| buf[x] == '\n' ) && x == 0 ){z = 0; o = 0;}
              	else if(buf[x] == ' ' || buf[x] == '\n' || buf[x] == '.' || buf[x] == '?' || buf[x] == ',' || buf[x] == ';' || buf[x] == '!'){
                                        aux = (x - z) + 1;
                                        for(i = 0; i < caracteres; i++){
           	                          	if (buf[aux + i] == cambiar[i]){o++;}
                                        }

					if (o == caracteres){
						o = 0;
						for(i = 0; i < z; i++){
							buf[aux+i] = toupper (buf[aux+i]);
                                                }
                                        }
			z = 0;
		}
		o = 0; 
       	}
	
	return buf;
}


int contar(char buffer[1024], int leido){	
	int x;
	int i = 0;
                
	for(x=0;x<leido;x++){
        	if((buffer[x] == ' '||  buffer[x] == '\n') && x == 0){i--;} //No me cuenta como palabra el espacio o el enter inicial
                	else{
             			if(buffer[x] == ' ' ||  buffer[x] == '\n' ||  buffer[x] == '.'){i++;}
                	}
	}             
	return i;
}


