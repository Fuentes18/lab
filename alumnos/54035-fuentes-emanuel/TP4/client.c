#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv) {
	//Obtengo argumentos
	int opt;
	char* hostname;
	char* palabra;
	int flag = 0;

	while ((opt = getopt(argc, argv, "u:t:")) != -1) {
		switch (opt) {
			case 'u':
				flag = flag + 1;
				hostname = optarg;
				break;
			case 't':
				flag = flag + 1;
				palabra = optarg;
				break;
			}
	}
	
	if(flag != 2) {
		printf("No se detecto argumento:\n-u host\n-t palabra\n");
		return -1;
	}

	//Fin argumentos

	int fd, leido, conectado;
	char buff [1000];
	struct sockaddr_in procrem={};
	fd = socket (AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror ("error socket");
		return -1;
	}
	procrem.sin_family = AF_INET;
	procrem.sin_port = htons (8080); 
	inet_pton(AF_INET,"127.0.0.1",&procrem.sin_addr);
	conectado = connect (fd, (struct sockaddr *) &procrem, sizeof procrem);
	if (conectado < 0) {
		perror ("error connect");
		return -1;
	}
	
	memset(buff,0,100);
	strncat(buff,hostname,strlen(hostname));
	strncat(buff," ",1);
	strncat(buff, palabra,strlen(palabra));

	write (fd,&buff,sizeof buff);
        
	leido = read (fd, buff, sizeof buff);

        if (leido < 0) {
                perror ("error read");
                return -1;
        }

	write (1, buff, leido);
	return 0;
}
