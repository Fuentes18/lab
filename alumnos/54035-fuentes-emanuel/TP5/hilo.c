#include "obtener_solicitud.c"
#include "size_archivo.c"
pthread_mutex_t sincro = PTHREAD_MUTEX_INITIALIZER;
char* ruta;

void *hilo(void *socket_desc){
  int sock = *(int*)socket_desc;
  
  pthread_mutex_unlock(&sincro);
	char aux[100];
	int fd_archivo, fd_index, size;
	char solicitud[2000];
	char client_message[2000];

	memset(client_message,0,strlen(client_message));
	memset(solicitud,0,strlen(solicitud));

	char* ok = "HTTP/1.1 200 OK\n\n";
	char* bad = "HTTP/1.1 404 Not Found\n\n";

	if (read(sock,client_message,2000) > 0){

		obtener_solicitud(client_message, solicitud);
		strncat(aux,ruta,strlen(ruta));
		strncat(aux,solicitud,strlen(solicitud));

		fd_archivo = open(aux, O_RDONLY);

		if(fd_archivo > 0){
			size = size_archivo(fd_archivo,solicitud);
			write(sock, ok, strlen(ok));
			sendfile(sock, fd_archivo, NULL,size);
		}else{
			write(sock, bad, strlen(bad));
		}

		close(fd_archivo);
		memset(aux, 0, sizeof aux);
	}

	//memset(&param, 0, sizeof param);
	memset(solicitud,0,strlen(solicitud));
	memset(client_message,0,strlen(client_message));
	close(sock);
	pthread_exit(NULL);
}
