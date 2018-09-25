#include "librerias.h"

int contador(char* buffer){
	int x;
	int i = 0;
             
	for(x=0;x<1024;x++){
     		if(buffer[x] == ' ' ||  buffer[x] == '\n'||  buffer[x] == '.'){i++;}
	}             
	return i;

}	

