#ifndef __SERVEUR__
#define __SERVEUR__

#include "main.h"

int analyseRequest(char*, char*, Store*, int);
int writeResponce(int, char*);
int splitParams(char*, char**);
int isSepa(char);

# endif /* __SERVEUR__ */
