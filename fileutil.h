#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int isregularfile(char*);
int numberoffiles(char*);
int listdir(char*, char**);
int filesize(char*);
