#include "librerias.h"

int procesar(char* archivo){
	
	int pid, pid2, leido, fichero;
	int x = 0;
	int aux = 0;

	char buffer[50];
	char buffer_punto[1024];
	char *buffer_mayu;

	int *ptr_sema;
	int *ptr_fin;
	char *ptr_archivo;

	sem_t *sema_listo1, *sema_listo2, *sema_termine1, *sema_termine2;

	//Abro los ficheros para leer.  
	if (archivo != NULL){ 		
		fichero = open (archivo,O_RDWR);
	}else{ 
		fichero = STDIN_FILENO;
	}
	
	ptr_sema = mmap(NULL,5 * sizeof (sem_t),PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,-1, 0);
	ptr_archivo = (char *) mmap(NULL,1024,PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,-1, 0);
	ptr_fin = mmap(NULL,sizeof (int),PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,-1, 0);
	
	sema_listo1 = (sem_t *)ptr_sema + sizeof sema_listo1;
	sema_listo2 = (sem_t *)ptr_sema + 2* sizeof sema_listo2; 
	sema_termine1 = (sem_t *)ptr_sema + 3* sizeof sema_termine1;
	sema_termine2 = (sem_t *)ptr_sema + 4* sizeof sema_termine2; 

	sem_init(sema_listo1,1,0);
	sem_init(sema_listo2,1,0);
	sem_init(sema_termine1,1,0);	
	sem_init(sema_termine2,1,0);
	
	pid = fork();
	if (pid){pid2 = fork();}
	

	//PROCESO 1

	if( pid == 0){
		printf("\n------------TEXTO CONVERTIDO A MAYUSCULAS------------\n");
		while(*ptr_fin == 1){	
			sem_wait(sema_listo1);
			buffer_mayu = mayusculas(ptr_archivo, strlen(ptr_archivo),6,"before");
			buffer_mayu = mayusculas(buffer_mayu, strlen(buffer_mayu),7,"without");	
			buffer_mayu = mayusculas(buffer_mayu, strlen(buffer_mayu),3,"the");			
			printf("%s\n", buffer_mayu);
			sem_post(sema_termine1);
		}
		printf("termine");
		
		return 0;
	}

	//PROCESO 2

	if( pid2 == 0){
		int palabras = 0;
		while(*ptr_fin == 1){				
			sem_wait(sema_listo2);	
			printf("\nSe leyó %d palabras en total.\n", contador(ptr_archivo));
			sem_post(sema_termine2);
		}

		return 0;
	}

	*ptr_fin = 1;
	//PADRE
	while((leido = read(fichero,&buffer,sizeof buffer)) > 0 ){
		for(x=0;x < leido ;x++){ //ESCRIBO HASTA ENCONTRAR UN PUNTO o UN ENTER
                       	buffer_punto[aux] = buffer[x];
			if (buffer_punto[aux] == '.' ){
				
				strncpy(ptr_archivo, buffer_punto, aux+1);
				aux = 0;				
					
				sem_post(sema_listo1);
				sem_post(sema_listo2);
				
				sem_wait(sema_termine1);
				sem_wait(sema_termine2);
												
				memset(buffer_punto,0, 1024);
				memset(ptr_archivo,0, 1024);
				
			 }else{
				aux++;
			}
                }
	}
	*ptr_fin = 0;

	return 0;	
}
