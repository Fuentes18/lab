int size_archivo(int fd_archivo, char* solicitud) {
	int fsize;
	struct stat file_stat;

	if (fstat(fd_archivo, &file_stat) < 0){
		return -1;
	}

	fsize = file_stat.st_size;

	return fsize;
}
