/*
 * serveur.c : serveur de l'objet connecté
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>

#include "capteur.h"

#define LBUF 512

typedef struct Store Store;
struct Store {
    pthread_t threadCapteur;
    pthread_t threadServeur;

    pthread_mutex_t mutexCapteur;
    char* address;
    char* port;
    unsigned int frequence;

    Capteur capteur;
    Capteur capteurOld;
};

static Store store =
{
    .frequence = 1000,       // millisecondes
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

    while (1) {
        /* Debut de la zone protegee. */
        pthread_mutex_lock (& store.mutexCapteur);

        if (!isFirst)
            memcpy(capteurOld, capteur, sizeof *capteurOld);
        else
            isFirst = 0;
		capteurCheck(capteur);
		calcCpuPcent(&capteur->cpu, &capteurOld->cpu);

        store.capteur.net.debitDown = (long) ((double) (store.capteur.net.totalDown - store.capteurOld.net.totalDown) /(double)( (double) (store.frequence)/1000));
        store.capteur.net.debitUp = (long) ((double) (store.capteur.net.totalUp - store.capteurOld.net.totalUp) / (double)( (double) (store.frequence)/1000));

		//printf("----------------------------------------------------------\n");
		//printf("Le taux processeur est de %g\n", store.capteur.cpu.pcentUsed);
		//printf("L'utilisation de la RAM est de %g % (%ld/%ld Mo)\n", ram.pcentUsed, ram.used / 1024, ram.total / 1024);
		//printf("L'utilisation du Swap est de %g % (%ld/%ld Mo)\n", swap.pcentUsed, swap.used / 1024, swap.total / 1024);
		//printf("Le trafic reseau total est de %g Mo en DL et de %g Mo en UL\n", (double) net.totalDown/1024/1024, (double) net.totalUp/1024/1024);
		//printf("Le trafic reseau est de %g Mo/s en DL et de %g Mo/s en UL\n",
        //    (double) store.capteur.net.debitDown/1024/1024, (double) store.capteur.net.debitUp/1024/1024);
		//printf("time %d\n", time(NULL));
		pthread_mutex_unlock (& store.mutexCapteur);
        /* Fin de la zone protegee. */
		usleep(store.frequence * 1000);
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
    sprintf(data, "{\"cpu\": { \"user\": %ld, \"nice\": %ld, \"system\": %ld, \"idle\": %ld, \"pcentUsed\": %g },\"ram\": { \"total\": %ld, \"free\": %ld, \"buffers\": %ld, \"cached\": %ld, \"used\": %ld, \"pcentUsed\": %g },\"swap\": { \"total\": %ld, \"free\": %ld, \"cached\": %ld, \"used\": %ld, \"pcentUsed\": %g },\"network\": { \"totalDown\": %ld, \"totalUp\": %ld, \"debitDown\": %ld, \"debitUp\": %ld },\"time\": %ld}",
        store.capteur.cpu.user, store.capteur.cpu.nice, store.capteur.cpu.system, store.capteur.cpu.idle, store.capteur.cpu.pcentUsed,
        store.capteur.ram.total, store.capteur.ram.free, store.capteur.ram.buffers, store.capteur.ram.cached, store.capteur.ram.used, store.capteur.ram.pcentUsed,
        store.capteur.swap.total, store.capteur.swap.free, store.capteur.swap.cached, store.capteur.swap.used, store.capteur.swap.pcentUsed,
        store.capteur.net.totalDown, store.capteur.net.totalUp, store.capteur.net.debitDown, store.capteur.net.debitUp,
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




