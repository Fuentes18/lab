#include <signal.h>
#include "proceso.c"

int main(int argc, char **argv) {

	//Obtengo argumentos
	int opt;
	int port;
	int flag = 0;

	while ((opt = getopt(argc, argv, "p:")) != -1) {
		switch (opt) {
			case 'p':
				flag = flag + 1;
				port = atoi(optarg);
				break;
			}
	}
	
	if(flag != 1) {
		printf("No se detecto argumento:\n-p puerto\n");
		return -1;
	}

	//Fin argumentos

	int fd, connfd, pid;	

	struct sockaddr_in procrem={};
	fd = socket (AF_INET, SOCK_STREAM, 0);

	if (fd < 0) {
		perror ("Error en el socket");
		return -1;
	}	

	procrem.sin_family = AF_INET;
	procrem.sin_port = htons(port);
	inet_pton(AF_INET,"127.0.0.1",&procrem.sin_addr);
	if ((bind(fd, (struct sockaddr *) &procrem, sizeof procrem)) <0){
		perror ("Error en el bind");
		return -1;
	}

	listen(fd,5); 
	signal(SIGCHLD, SIG_IGN);

	while((connfd = accept(fd,NULL,0))>0){
		proceso(connfd);
	}
	close(connfd);
	return 0;
}

