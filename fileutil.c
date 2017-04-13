#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#define _MAXSTRING_ 50

// Esta funcion devuelve "TRUE" si 'dir' es un nombre que no corresponde a un 
// directorio. https://goo.gl/EV2VrY
int isregularfile(char *dir) {
	struct stat path_stat;
	stat(dir, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

// Esta funcion devuelve el numero de archivos (no de directorios), que contiene
// el directorio 'dir'
int numberoffiles(char* dir) {
	// Definicion e inicializacion de variables, entre ellas la que contara
	// el numero de archivos que tiene el directorio
	int count = 0;
	DIR *dp;
	struct dirent *ep;

	// Se valida si es un archivo regular. Si es archivo regular no tiene
	// archivos en su interior la funcion termina
	if (isregularfile(dir)) {
		perror("numberoffiles - Es un archivo regular\n");
		return -1;
	}
	// Se valida si se puede abrir el directorio
	dp = opendir(dir);
	if (dp == NULL) {
		perror("numberoffiles - No se pudo abrir el directorio");
		return -1;
	}
	// Se recorre el directorio y si el elemento en el directorio es un
	// archivo regular se incrementa el contador
	while (ep = readdir(dp)) {
		if (isregularfile(ep->d_name))
			count++;
	}
	// Se cierra la variable que apunta al directorio
	closedir(dp);
	// Se retorna el numero de archivos en el directorio
	return count;
}

// Esta funcion recibe el nombre de un directorio, 'dir', y en la variable
// 'result' entrega un vector de cadena de caracteres con los nombres de los 
// archivos en el directorio 'dir'. 
// El valor retornado es
// -1, si el directorio no es un directorio o no se pudo abrir
// otro valor, es la cantidad de archivos en el directorio
//
// Fuente: https://goo.gl/aC3tlq
int listdir(char *dir, char** result) {
	// Definicion de variables
	DIR *dp;
	struct dirent *ep;
	int count;
	
	// Se valida que se pueda abrir el directorio
	//printf("Buscando en %s\n",dir);
	dp = opendir(dir);
	if (dp == NULL) {
		perror("listdir - No se pudo abrir el directorio");
		return -1;
	}
	// Se inicializara, de ser necesario, la variable donde se almacenaran
	// el vector de cadena de caracteres que contendra los archivos que 
	// se encuentren en el directorio 'dir'
	if (result == NULL) {
		count = numberoffiles(dir); 
		if (count == -1) {
			perror("listdir - No se encontraron archivos en el directorio\n");
			closedir(dp);
			return -1;
		}
		result = (char**)malloc(count*sizeof(char*));
	}
	// Se recorrera el directorio, y en cada archivo que no sea un 
	// directorio se almacenara este nombre en el vector de cadena de 
	// caracteres resultado
	count = 0;
	while (ep = readdir(dp)) {
		char currentPath[FILENAME_MAX];
		struct stat statbuf;
		if (strcmp(ep->d_name,".") == 0 || strcmp(ep->d_name,"..") == 0)
			continue;
		strcpy(currentPath, dir);
		strcat(currentPath, "/");
		strcat(currentPath, ep->d_name);
		//printf("[%s]]\n",currentPath);
		if (stat(currentPath, &statbuf) == -1){
			perror("stat");
			continue;
		}
		//if (!isregularfile(ep->d_name))
		if (S_ISDIR(statbuf.st_mode))
			continue;
		//printf("[PASO] %s\n",currentPath);
		if (result[count] == NULL) {
			result[count] = (char*)malloc(sizeof(char)*FILENAME_MAX);
		}
		strcpy(result[count],currentPath);
		count++;
	}
	closedir(dp);
	return count;
}

// Este metodo recibe un nombre de un archivo y calcula su tamano en bytes
// -1, si no se pudo calcular el tamano
// 
int filesize(char *filename) {
	// Definicion e inicializacion de variables
	FILE *fp;
	int count = 0;
	char mensaje[256];

	// Se abre el archivo y se contara caracter por caracter el numero 
	// de bytes del archivo
	fp = fopen(filename,"r");
	if (fp == NULL) {
		sprintf(mensaje,"filesize - No se pudo abrir archivo [%s]\n",filename);
		perror(mensaje);
		return -1;
	}
	while (fgetc(fp) != EOF) {
		count++;
	}
	fclose(fp);
	return count;
}
