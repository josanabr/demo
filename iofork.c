#include "fileutil.h"

#define VECTORSIZE 50
#define MAXSTRING 50

int main(int argc, char** argv) {
	char **cadenas;
	int count;
	int i;
	int total = 0;
	char *directorio;

	printf("Estudiante_1: 201512345\n");
	printf("Estudiante_2: 201567890\n");
	// Inicializacion de la matriz 'cadenas'
	cadenas = (char**)malloc(sizeof(char*)*VECTORSIZE); 
	for (i = 0; i < VECTORSIZE; i++) 
		cadenas[i] = (char*)malloc(sizeof(char)*FILENAME_MAX);
	// Se consulta por los archivos en el directorio
	if (argc == 2) 
		directorio = argv[1];
	else 
		directorio = "./";
	count = listdir(directorio,cadenas);
	if (count == -1) 
		return -1;
	// Se imprime el contenido de 'cadenas'
	for (i = 0; i < count; i++) {
		int fs = filesize(cadenas[i]); 
		if (fs == -1) continue;
		//printf("[%s] TAMANO %d\n",cadenas[i], fs);
		total += fs;
	}

	printf("Total archivos: %d\n",count);
	printf("Total bytes: %d\n",total);
	return 0;
}
