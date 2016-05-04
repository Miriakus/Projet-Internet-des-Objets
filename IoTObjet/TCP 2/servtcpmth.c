/*
 * servtcp.c : exemple de serveur socket mode connecte
 * avec multi-threading pour creer un thread à chaque connexion
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

struct sockaddr_in Sin = {AF_INET}; /* le reste est nul */

struct elt_dns
{
   char nom[30];
   char adip[20];
} annuaire [] = {
    "chaource", "192.168.99.1",
    "roquefort", "192.168.99.2",
    "comte", "192.168.99.3",
    "reblochon", "192.168.99.4",
    "beaufort", "192.168.99.5",
    "abondance", "192.168.99.6",
    "maroilles", "192.168.99.7",
    "livarot", "192.168.99.8",
    "morbier", "192.168.99.9",
    "brie", "192.168.99.10",
    "", ""
};

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

#define LBUF 100
void * service(void * p)
{
long sid;
int n, i;
char buf[LBUF];
    sid = (long)p;
    n = readlig((int)sid,buf,LBUF);
    if (n < 0) {
       perror("readlig");
       return NULL;
    }
    i = 0;
    while (strlen(annuaire[i].nom) > 0) {
        if (strcmp(buf,annuaire[i].nom) == 0) {
           write(sid,annuaire[i].adip,20);
           close((int)sid);
           return NULL;
        }
        i++;
    }
    write(sid,"erreur !! ", 11);
    close(sid);
    return NULL;
}

int main(int N, char *P[])
{
int ln, sock, nsock, err;
pthread_attr_t attr;
pthread_t thid;

    /* creation du socket */
    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(1);
    }
    /* ATTACHEMENT AU PORT */
    if(bind(sock,(struct sockaddr*)&Sin, sizeof(Sin)) < 0) {
        perror("bind");
        exit(2);
    }
    ln = sizeof(Sin);
    if (getsockname(sock,(struct sockaddr*)&Sin,(socklen_t*)&ln) < 0) {
        perror("getsockname");
        exit(3);
    }
    printf("Le serveur est attache au port %u\n",ntohs(Sin.sin_port));
    /* definition du nb d'appels simultanes */
    if (listen(sock,5) < 0) {
        perror("listen");
        exit(4);
    }
    /* initialisation de l'attribut pour creer les thread afin que chaque
       thread soit indépendante (pas de pthread_join() possible !!)
    */
    if ((err = pthread_attr_init(&attr)) != 0) {
       fprintf(stderr,"Erreur %d sur pthread_attr_init()\n",err);
       exit(5);
    }
    /* on ajoute dans l'attribut le fait que le thread sera independant*/
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    /* boucle d'attente */
    for (;;) {
        if((nsock=accept(sock,(struct sockaddr*)&Sin,(socklen_t*)&ln))<0) {
            perror("accept");
            exit(6);
        }
        /* creation d'un thread qui va executer la fct service */
        if ((err=pthread_create(&thid,&attr,service,(void*)((long)nsock))) != 0) {
            fprintf(stderr,"Erreur %d sur pthread_create()\n",err);
            exit(7);
        }
    }
}




