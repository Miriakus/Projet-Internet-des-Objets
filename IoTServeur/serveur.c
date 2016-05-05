/*
 * servtcp.c : exemple de serveur socket mode connecte
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "capteur.h"

struct sockaddr_in Sin = {AF_INET}; /* le reste est nul */

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

#define LBUF 255
void service(int sid)
{
Cpu cpu;
Ram ram;
Swap swap;
Network net;
int n, i;
char buf[LBUF], data[LBUF];
    n = readlig(sid,buf,LBUF);
    if (n < 0) {
       perror("readlig");
       return;
    }
    i = 0;
    fprintf(stderr, "Recu : %s\n", buf);

    cpu = cpuCheck();
    ram = ramCheck();
    swap = swapCheck();
    net = networkCheck();

    sprintf(data, "{ram: %g, swap: %g, download: %ld, upload: %ld}", ram.pcentUsed, swap.pcentUsed, net.totalDown, net.totalUp);

    fprintf(stderr, "Emit : %s\n", data);

    write(sid,data, 11);
    close(sid);
}

int main(int N, char *P[])
{
int ln, sock, nsock;
    /* creation du socket */
    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(1);
    }
    Sin.sin_port = htons(42000);
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
    /* boucle d'attente */
    for (;;) {
        if((nsock=accept(sock,(struct sockaddr*)&Sin,(socklen_t*)&ln))<0) {
            perror("accept");
            exit(5);
        }
        service(nsock);
    }
}




