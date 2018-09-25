#include "librerias.h"

char* mayusculas(char* buf, int leido, int caracteres, char *objetivo){
	int x =0;
	int i = 0;
	int hit = 0;
	
	char cambiar_palabra[caracteres];
	strcpy(cambiar_palabra, objetivo);
	
	for (x = 0;x<leido;x++){
		//Voy analizando caracter por caracter, si encuentro un espacio y tengo (hit = caracteres), encontre la palabra!
		if(buf[x] == ' ' || buf[x] == '\n' || buf[x] == '.' || buf[x] == '?' || buf[x] == ',' || buf[x] == ';' || buf[x] == '!'){
			if(hit == caracteres){				
				for(i = 0; i < caracteres; i++){
					buf[x - caracteres + i] = toupper (cambiar_palabra[i]);
                		}

				hit = 0;
				i = 0;
			}	
		}	

		if(buf[x] == cambiar_palabra[i]){
			hit++;
			i++;
		}else{
			hit = 0; 
			i =0;
		}
	 
	}
	
	return buf;
}
