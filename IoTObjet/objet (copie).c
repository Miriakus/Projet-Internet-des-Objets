#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


//#include "capteur.h"

#define LBUF 255

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main2(int argc, char *argv[])
{
	/*Cpu cpu;
    Ram ram;
    Swap swap;
	Network net;*/

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    printf("plop");
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Please enter the message: ");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);
		/*cpu = cpuCheck();
		ram = ramCheck();
		swap = swapCheck();
		net = networkCheck();
		sprintf(buffer, "RAM: %g - SWAP: %g", ram.pcentUsed, swap.pcentUsed);*/
		printf("info : %s", buffer);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}

int main(int N, char *P[]) {
    /*Cpu cpu;
    Ram ram;
    Swap swap;
	Network net;
*/
	int sock;
	struct sockaddr_in sin;
	struct hostent *h;
	char nom[LBUF], addr[LBUF];

    if (N != 3 ) {
        fprintf(stderr,"Utilisation : %s nom_serveur port\n",P[0]);
        exit(1);
    }
    /* initialisation socket */
    if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(2);
    }
    if(!(h=gethostbyname(P[1]))) {
        perror("gethostbyname");
        exit(3);
    }
    /* initialisation de sin */
    bzero(&sin,sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(h->h_addr,&sin.sin_addr,h->h_length);
    sin.sin_port = htons(atoi(P[2]));
    if (connect(sock,(struct sockaddr*)&sin,sizeof(sin)) < 0) {
        perror("connect");
        exit(4);
    }
	printf("plop");
	//while (1==1) {
		/*cpu = cpuCheck();
		ram = ramCheck();
		swap = swapCheck();
		net = networkCheck();

		printf("plop");
		sprintf(nom, "RAM: %g - SWAP: %g", ram.pcentUsed, swap.pcentUsed);
		printf("info : %s", nom);
		write(sock,nom,strlen(nom));
		/*printf("----------------------------------------------------------\n");
		printf("Le nombre de tick processeur est %ld user, %ld idle\n", cpu.user, cpu.idle);
		printf("L'utilisation de la RAM est de %g % (%ld/%ld Mo)\n", ram.pcentUsed, ram.used / 1024, ram.total / 1024);
		printf("L'utilisation du Swap est de %g % (%ld/%ld Mo)\n", swap.pcentUsed, swap.used / 1024, swap.total / 1024);
		printf("Le trafic reseau total est de %g Mo en DL et de %g Mo en UL\n", (double) net.totalDown/1024/1024, (double) net.totalUp/1024/1024);
		printf("time %d\n", time(NULL));*/
		read(sock,addr,LBUF);
		printf("Retour : %s\n",addr);
		close(sock);
		usleep(1000000);
	//}
    /*printf("Donner un nom de machine\n");
    fgets(nom,LBUF,stdin);
    write(sock,nom,strlen(nom));
    read(sock,addr,LBUF);
    nom[strlen(nom)-1]='\0';
    if (strncmp(addr,"erreur !! ",9) == 0)
       printf("La machine %s n'existe pas !\n", nom);
    else
       printf("La machine %s a pour adresse IP %s\n",nom,addr);*/
    //close(sock);
}
