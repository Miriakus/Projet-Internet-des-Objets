/*
 * serveur.c : serveur de l'objet connecté
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#include "capteur.h"

#define LBUF 1024

typedef struct Store Store;
struct Store {
    pthread_t threadCapteur;
    pthread_t threadServeur;

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

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

static void * threadCapteur (void *data)
{
    int isFirst = 1;
    Capteur *capteur = &store.capteur;
    Capteur *capteurOld = &store.capteurOld;
    clock_t timer;

    while (1) {
        timer = clock();
        /* Debut de la zone protegee. */
        pthread_mutex_lock (& store.mutexCapteur);

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
		pthread_mutex_unlock (& store.mutexCapteur);
		/* Fin de la zone protegee. */
		usleep(store.frequence * 1000 - (clock()-timer));
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

void service(int sid)
{
    char buf[LBUF], data[LBUF];
    if (readlig(sid,buf,LBUF) < 0) {
       perror("readRequest");
       return;
    }
    fprintf(stderr, "Recu : %s\n", buf);

    /* Debut de la zone protegee. */
    pthread_mutex_lock (& store.mutexCapteur);

    Cpu *cpu = &store.capteur.cpu;
    Ram *ram = &store.capteur.ram;
    Swap *swap = &store.capteur.swap;
    Disk *disk = &store.capteur.disk;
    Network *network = &store.capteur.network;

    sprintf(data, "{ \"cpu\": { \"user\": %ld, \"nice\": %ld, \"system\": %ld, \"idle\": %ld, \"pcentUsed\": %g }, \"ram\": { \"total\": %ld, \"free\": %ld, \"buffers\": %ld, \"cached\": %ld, \"used\": %ld, \"pcentUsed\": %g }, \"swap\": { \"total\": %ld, \"free\": %ld, \"cached\": %ld, \"used\": %ld, \"pcentUsed\": %g }, \"disk\": { \"totalRead\": %ld, \"totalWrite\": %ld, \"totalTimeActive\": %ld, \"debitRead\": %ld, \"debitWrite\": %ld, \"pcentActive\": %g }, \"network\": { \"totalDown\": %ld, \"totalUp\": %ld, \"debitDown\": %ld, \"debitUp\": %ld },\"time\": %ld }",
        cpu->user, cpu->nice, cpu->system, cpu->idle, cpu->pcentUsed,
        ram->total, ram->free, ram->buffers, ram->cached, ram->used, ram->pcentUsed,
        swap->total, swap->free, swap->cached, swap->used, swap->pcentUsed,
        disk->totalRead, disk->totalWrite, disk->totalTimeActive, disk->debitRead, disk->debitWrite, disk->pcentActive,
        network->totalDown, network->totalUp, network->debitDown, network->debitUp,
        store.capteur.time);

    pthread_mutex_unlock (& store.mutexCapteur);
    /* Fin de la zone protegee. */
    fprintf(stderr, "Emit : %s\n", data);

    if (write(sid,data, strlen(data)) < 0) {
        perror("writeResponce");
        return;
    }
    close(sid);
}

int main(int N, char *P[])
{
    // Creation des threads
    if (pthread_create (& store.threadCapteur, NULL, threadCapteur, NULL) != 0)
        error("pthread");
    else
        printf("Creation du thread d'acquisition des capteurs !\n");

    struct sockaddr_in Sin = {AF_INET}; /* le reste est nul */
    int ln, sock, nsock;
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
    /* boucle d'attente */
    for (;;) {
        if ((nsock=accept(sock,(struct sockaddr*)&Sin,(socklen_t*)&ln))<0)
            error("accept");
        service(nsock);
    }
    pthread_join (store.threadCapteur, NULL);

    return 0;
}




