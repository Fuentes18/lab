#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int get_info(char* hostname, char* ip, char* url, char* palabra, char* encontrado) {
	int fd, conectado;
	char* solicitud;
	char* busqueda;
	char sendline[2000], recvline[2000];
	size_t n;

	struct sockaddr_in procrem={};
	fd = socket (AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror ("error socket");
		return -1;
	}
	procrem.sin_family = AF_INET;
	procrem.sin_port = htons (80); 
	inet_pton(AF_INET,ip,&procrem.sin_addr);
	conectado = connect (fd, (struct sockaddr *) &procrem, sizeof procrem);
	if (conectado < 0) {
		perror ("error connect");
		return -1;
	}	

	sprintf(sendline, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n",url,hostname);

	if (write(fd, sendline, strlen(sendline))>= 0) 
	{
		while ((n = read(fd, recvline, sizeof recvline)) > 0) 
		{
			busqueda = strstr(recvline, palabra);
			if(busqueda != NULL){
				busqueda = strtok_r(busqueda,"<", &busqueda);
				strcpy(encontrado,busqueda);				
				return 0;			
			}
			
		} 		
	} 

	return -1;
}
