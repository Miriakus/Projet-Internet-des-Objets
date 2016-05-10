#ifndef __MAIN__
#define __MAIN__

#include <pthread.h>

#include "capteur.h"

typedef struct Store Store;
struct Store {
    pthread_t threadCapteur;
    pthread_t threadTCP;
    pthread_attr_t threadAttr;
    pthread_mutex_t mutexCapteur;

    char* address;
    char* port;
    unsigned int frequence;         // millisecondes

    Capteur capteur;
    Capteur capteurOld;
};

static Store store =
{
    .frequence = 1000,
    .mutexCapteur = PTHREAD_MUTEX_INITIALIZER
};

# endif /* __MAIN__ */

