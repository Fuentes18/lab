int obtener_solicitud(char* buff, char* solicitud){
	char* busqueda;

	busqueda = strtok(buff, "/");
	busqueda = strtok(NULL, " ");

	strncpy(solicitud, busqueda, strlen(busqueda));
	return 0;
}
