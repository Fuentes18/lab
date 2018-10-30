#include "get_host_info.c"
#include "get_info.c"

int proceso(int connfd){		
	int pid = fork();

	if(pid == 0){
		char buff [1000];
		char* hostname = NULL;		
		char* url = NULL;
		char* palabra = NULL;
		char ip[100];
		char encontrado[100];
		int aux;

		memset(ip,0,strlen(ip));
		memset(encontrado,0,strlen(encontrado));

		read(connfd,buff,sizeof buff); 	
		aux = strlen(buff); 

		hostname = strtok(buff, "/");

		if(strlen(hostname) == aux){
			hostname = strtok(buff," ");
		}else{
			url = strtok(NULL," ");
		}		

		palabra= strtok(NULL," ");

		get_host_info(hostname, ip, url, palabra);
		get_info(hostname, ip, url, palabra, encontrado);

		printf("\nSolicitud entrante.... Hostname: %s, Ip: %s, Palabra: %s\n",hostname,ip,palabra);
		
		write(connfd,encontrado,strlen(encontrado));

		return 0;
	}

	return 0;
}
