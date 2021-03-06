/*
 * objet.c : main de l'objet connecté
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#include "main.h"
#include "capteur.h"
#include "serveur.h"

#define LBUF 1024

static Store store =
{
    .frequence = 1000,
    .mutexCapteur = PTHREAD_MUTEX_INITIALIZER
};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

static void * threadCapteur(void *params)
{
    int isFirst = 1;
    Capteur *capteur = &store.capteur;
    Capteur *capteurOld = &store.capteurOld;
    clock_t timer;
    unsigned int frequence;

    while (1) {
        timer = clock();
        /* Debut de la zone protegee. */
        pthread_mutex_lock (&store.mutexCapteur);

        if (isFirst) {
            isFirst = 0;
            capteurCheck(capteur);

            capteur->cpu.pcentUsed = 0.;
            capteur->disk.debitRead = 0;
            capteur->disk.debitWrite = 0;
            capteur->disk.pcentActive = 0.;
            capteur->network.debitDown = 0;
            capteur->network.debitUp = 0;
        } else {
            memcpy(capteurOld, capteur, sizeof *capteurOld);
            capteurCheck(capteur);

            calcCpuPcent(&capteur->cpu, &capteurOld->cpu);
            calcDiskDebit(&capteur->disk, &capteurOld->disk, store.frequence);
            calcNetworkDebit(&capteur->network, &capteurOld->network, store.frequence);
        }
        frequence = store.frequence;
        //printf("sec : %ld , usec: %ld , diff: %ld - %d\n", capteur->time.sec, capteur->time.microsec,
        //    capteur->time.microsec-capteurOld->time.microsec, store.frequence);
		pthread_mutex_unlock (&store.mutexCapteur);
		/* Fin de la zone protegee. */
		usleep(frequence * 1000 - (clock()-timer));
   }
   return NULL;
}

int readlig(int fd, char *b, int max)
{
int n;
char c;
    for (n=0; n<max; n++) {
        if(read(fd, &c, 1) <= 0) break;
        if (c == '\n') break;
        *b++ = c;
    }
    *b = '\0';
    return(n);
}

static void * threadTCP(void *params)
{
    char request[LBUF], response[LBUF];
    int sid = (int) params;
    if (readlig(sid,request,LBUF) < 0) {
        close(sid);
       perror("readRequest");
       return NULL;
    }
    fprintf(stderr, "Recu : %s\n", request);

    analyseRequest(request, response, &store, sid);

    close(sid);
    return NULL;
}

int main(int N, char *P[])
{
    struct sockaddr_in Sin = {AF_INET}; /* le reste est nul */
    int ln, sock, nsock, err;
    /* creation du socket */
    if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
        error("socket");
    /* ATTACHEMENT AU PORT */
    Sin.sin_port = htons(42000);
    if (bind(sock,(struct sockaddr*)&Sin, sizeof(Sin)) < 0)
        error("bind");
    ln = sizeof(Sin);
    if (getsockname(sock,(struct sockaddr*)&Sin,(socklen_t*)&ln) < 0)
        error("getsockname");
    printf("Le serveur est attache au port %u\n",ntohs(Sin.sin_port));
    /* definition du nb d'appels simultanes */
    if (listen(sock,5) < 0)
        error("listen");

    /* initialisation de l'attribut pour creer les thread afin que chaque
        thread soit indépendante (pas de pthread_join() possible !!)
    */
    if ((err = pthread_attr_init(&store.threadAttr)) != 0) {
       fprintf(stderr,"Erreur %d sur pthread_attr_init()\n",err);
       exit(5);
    }
    /* on ajoute dans l'attribut le fait que le thread sera independant*/
    pthread_attr_setdetachstate(&store.threadAttr, PTHREAD_CREATE_DETACHED);

    // Creation des threads
    if (pthread_create (&store.threadCapteur, &store.threadAttr, threadCapteur, NULL) != 0)
        error("pthread");
    else
        printf("Creation du thread d'acquisition des capteurs !\n");

    /* boucle d'attente */
    for (;;) {
        if ((nsock=accept(sock,(struct sockaddr*)&Sin,(socklen_t*)&ln))<0)
            error("accept");
        /* creation d'un thread qui va executer la fct threadTCP */
        if ((err=pthread_create(&store.threadTCP, &store.threadAttr, threadTCP, (void*)((long)nsock))) != 0) {
            fprintf(stderr,"Erreur %d sur pthread_create()\n",err);
            exit(7);
        } else
            printf("Creation d'un thread de connexion !\n");
    }
    //pthread_join (store.threadCapteur, NULL);

    return 0;
}




