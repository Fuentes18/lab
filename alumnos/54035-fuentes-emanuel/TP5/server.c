#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include "hilo.c"

int main(int argc, char **argv) {
	pthread_mutex_unlock(&sincro);
	//Obtengo argumentos
	int opt;
	int fichero;
	char* archivo;
	char* puerto;
	int flag = 0;
	char buff[50];

	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				flag = flag + 1;
				archivo = optarg ;
				break;
			}
	}

	if(flag != 1) {
		printf("\nNo se especifico ningun elemento de configuracion, se procedera a tomar el server.conf...\n\n");
		fichero = open("server.conf",O_RDWR);
	}else{
		fichero = open(archivo,O_RDWR);
	}

	while((read(fichero,&buff,sizeof buff)) > 0 ){
		ruta = strtok(buff, "=");
		ruta = strtok(NULL, " ");
		puerto = strtok(NULL, "=");
		puerto = strtok(NULL, " ");
	}

	//Fin argumentos

	int fd, pid, connfd, retorno;
	int x = 0;
	pthread_t tid;
	char* server = "HTTP/1.1 500 Internal Server Error\n\n";

	struct sockaddr_in procrem={};
	fd = socket (AF_INET, SOCK_STREAM, 0);

	if (fd < 0) {
		perror ("Error en el socket.");
		return -1;
	}
	procrem.sin_family = AF_INET;
	procrem.sin_port = htons(atoi(puerto));
	inet_pton(AF_INET,"127.0.0.1",&procrem.sin_addr);
	int a;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if ((bind(fd, (struct sockaddr *) &procrem, sizeof procrem)) <0){
		perror ("Error en el bind.");
		return -1;
	}

	if (listen(fd,10) == -1){
		perror("Error en el listen.");
		close(fd);
		return -1;
	}
	//struct parametros param;
		//param.ruta = ruta;

	while((connfd = accept(fd,NULL,0))>0){

		//param.connfd = connfd;
		if( pthread_create(&tid, NULL, hilo, (void*) &connfd) < 0) {
			perror("No se pudo crear el hilo.");
			write(connfd, server, strlen(server));
			return -1;
		}
		pthread_detach(tid);
		pthread_mutex_lock(&sincro);
		//memset(&param, 0, sizeof param);
	}
	close(connfd);
	close(fd);
	return 0;
}
