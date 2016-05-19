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
#include <signal.h>

#include "main.h"
#include "capteur.h"
#include "serveur.h"

#define LBUF 1024
#define PORT 42000

static Store store =
        {
                .port = PORT,
                .frequence = 1000,
                .mutexCapteur = PTHREAD_MUTEX_INITIALIZER
        };

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

static void *threadCapteur(void *params)
{
    int isFirst = 1;
    unsigned int frequence;
    Capteur *capteur = &store.capteur;
    Capteur *capteurOld = &store.capteurOld;
    clock_t timer;

    while (1) {
        timer = clock();
        /* Debut de la zone protegee. */
        pthread_mutex_lock(&store.mutexCapteur);

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

        pthread_mutex_unlock(&store.mutexCapteur);
        /* Fin de la zone protegee. */
        usleep(frequence * 1000 - (clock() - timer));
    }
    return NULL;
}

int readlig(int fd, char *b, int max)
{
    int n;
    char c;
    for (n = 0; n < max; n++) {
        if (read(fd, &c, 1) <= 0) break;
        if (c == '\n') break;
        *b++ = c;
    }
    *b = '\0';
    return n;
}

static void *threadTCP(void *params)
{
    int sid = (int) params, interval = 0;
    char request[LBUF], response[LBUF];

    while (1) {
        bzero(request, sizeof request);
        if (readlig(sid, request, LBUF) < 0) {
            close(sid);
            perror("readRequest");
            return NULL;
        } else
            fprintf(stderr, "Recu : %s\n", request);

        interval = analyseRequest(request, response, &store, sid);

        if (interval) {
            if (pthread_create(&store.threadInterval, &store.threadAttr, threadInterval, params) != 0)
                error("pthread");
            else
                printf("Creation d'un thread d'envoi par interval !\n");
        } else {
            if (writeResponce(sid, response))
                return NULL;
        }
        usleep(1000);   // 1 ms
    }
}

void *threadInterval(void *params)
{
    int sid = (int) params;
    unsigned int frequence;
    char json[LBUF], response[LBUF];
    clock_t timer;

    while (1) {
        timer = clock();
        /* Debut de la zone protegee. */
        pthread_mutex_lock(&store.mutexCapteur);
        printJSON(json, &store.capteur);
        frequence = store.frequence;
        pthread_mutex_unlock(&store.mutexCapteur);
        /* Fin de la zone protegee. */
        sprintf(response, "<start|%s|end>", json);

        if (writeResponce(sid, response))
            return NULL;
        usleep(frequence * 1000 - (clock() - timer));
    }
}

int main(int N, char *P[])
{
    signal(SIGPIPE, SIG_IGN);
    if (N == 2) {
        if (strcmp(P[1], "-h") == 0 || strcmp(P[1], "--help") == 0) {
            printf("Utilisation : %s [PORT]\nPORT est un entier compris entre 1024 et 65535 (Par defaut 42000)\n", P[0]);
            exit(0);
        }
        unsigned int port = strtoul(P[1], NULL, 0);
        if (port >= 1024 && port <= 65535)
            store.port = port;
    }

    struct sockaddr_in Sin = {AF_INET}; /* le reste est nul */
    int ln, sock, nsock, err;
    /* creation du socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        error("Socket");

    /* ATTACHEMENT AU PORT */
    Sin.sin_port = htons(store.port);
    if (bind(sock, (struct sockaddr *) &Sin, sizeof(Sin)) < 0)
        error("Bind");

    ln = sizeof(Sin);
    if (getsockname(sock, (struct sockaddr *) &Sin, (socklen_t *) &ln) < 0)
        error("Get_socket_name");
    else
        printf("Le serveur est attache au port %u\n", ntohs(Sin.sin_port));
    /* definition du nb d'appels simultanes */
    if (listen(sock, 5) < 0)
        error("Listen");

    /* initialisation de l'attribut pour creer les thread afin que chaque
        thread soit indépendante (pas de pthread_join() possible !!)
    */
    if ((err = pthread_attr_init(&store.threadAttr)) != 0)
        error("Pthread_init_attributs");
    /* on ajoute dans l'attribut le fait que le thread sera independant*/
    pthread_attr_setdetachstate(&store.threadAttr, PTHREAD_CREATE_DETACHED);

    // Creation des threads
    if (pthread_create(&store.threadCapteur, &store.threadAttr, threadCapteur, NULL) != 0)
        error("Pthread_create");
    else
        printf("Creation du thread d'acquisition des capteurs !\n");

    // Boucle d'attente
    while (1) {
        if ((nsock = accept(sock, (struct sockaddr *) &Sin, (socklen_t *) &ln)) < 0)
            error("Accept");
        /* creation d'un thread qui va executer la fct threadTCP */
        if (pthread_create(&store.threadTCP, &store.threadAttr, threadTCP, (void *) ((long) nsock)) != 0)
            error("Pthread_create");
        else
            printf("Creation d'un thread pour une connexion !\n");
    }
    //pthread_join (store.threadCapteur, NULL);

    return 0;
}




