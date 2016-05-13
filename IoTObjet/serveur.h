#ifndef __SERVEUR__
#define __SERVEUR__

#include "main.h"

int isSepa(char);
int analyseRequest(char*, char*, Store*, int);
int splitParams(char*, char**);

# endif /* __SERVEUR__ */
