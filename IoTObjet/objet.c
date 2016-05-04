#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include "capteur.h"

#define LBUF 255


typedef struct Store Store;
struct Store {
    pthread_t threadAuto;
    pthread_t threadRequest;

    char* address;
    char* port;
    unsigned int frequence;
};

static Store store =
{
    .frequence = 1000       // millisecondes
};

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

static void * threadAuto (void *data)
{
    Cpu cpu;
    Ram ram;
    Swap swap;
	Network net;

	int sock;
	struct sockaddr_in sin;
	struct hostent *h;
	char nom[LBUF], addr[LBUF];
    while (1) {
        // initialisation socket
        if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
            perror("socket");
            exit(2);
        }
        if(!(h=gethostbyname(store.address))) {
            perror("gethostbyname");
            exit(3);
        }
        // initialisation de sin
        bzero(&sin,sizeof(sin));
        sin.sin_family = AF_INET;
        bcopy(h->h_addr,&sin.sin_addr,h->h_length);
        sin.sin_port = htons(atoi(store.port));
        if (connect(sock,(struct sockaddr*)&sin,sizeof(sin)) < 0) {
            perror("connect");
            exit(4);
        }
		cpu = cpuCheck();
		ram = ramCheck();
		swap = swapCheck();
		net = networkCheck();

		sprintf(nom, "USER: %ld - RAM: %g - SWAP: %g", cpu.user, ram.pcentUsed, swap.pcentUsed);
		//fprintf(stderr, "info : %s\n", nom);
		write(sock,nom,strlen(nom));
		/*printf("----------------------------------------------------------\n");
		printf("Le nombre de tick processeur est %ld user, %ld idle\n", cpu.user, cpu.idle);
		printf("L'utilisation de la RAM est de %g % (%ld/%ld Mo)\n", ram.pcentUsed, ram.used / 1024, ram.total / 1024);
		printf("L'utilisation du Swap est de %g % (%ld/%ld Mo)\n", swap.pcentUsed, swap.used / 1024, swap.total / 1024);
		printf("Le trafic reseau total est de %g Mo en DL et de %g Mo en UL\n", (double) net.totalDown/1024/1024, (double) net.totalUp/1024/1024);
		printf("time %d\n", time(NULL));*/
		//read(sock,addr,LBUF);
		//fprintf(stderr, "Retour : %s\n",addr);
		close(sock);
		usleep(store.frequence * 1000);
   }
   return NULL;
}


int main(int N, char *P[]) {
    Cpu cpu;
    Ram ram;
    Swap swap;
	Network net;

	int sock;
	struct sockaddr_in sin;
	struct hostent *h;
	char nom[LBUF], addr[LBUF];

    if (N != 3 ) {
        fprintf(stderr,"Utilisation : %s nom_serveur port\n",P[0]);
        exit(1);
    }

    store.address = P[1];
    store.port = P[2];

    /* Creation des threads. */
   printf ("Creation du thread d'envoi régulier !\n");
   if (pthread_create (& store.threadAuto, NULL, threadAuto, NULL) != 0) {
        perror("pthread");
        exit(10);
   }
   pthread_join (store.threadAuto, NULL);

    /*printf("Donner un nom de machine\n");
    fgets(nom,LBUF,stdin);
    write(sock,nom,strlen(nom));
    read(sock,addr,LBUF);
    nom[strlen(nom)-1]='\0';
    if (strncmp(addr,"erreur !! ",9) == 0)
       printf("La machine %s n'existe pas !\n", nom);
    else
       printf("La machine %s a pour adresse IP %s\n",nom,addr);*/
    close(sock);
}
