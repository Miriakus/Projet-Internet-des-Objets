#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serveur.h"
#include "capteur.h"

#define LBUF 1024

void analyseRequest(char *request, char *response, Store *store, int sid)
{
    char* params[255];
    int nbParams = splitParams(request, params);
    if (nbParams == 0) {
        sprintf(response, "ERROR : No request !");
        return;
    }
    if (strcmp(params[0], "CAPT_JSON") == 0) {
        /* Debut de la zone protegee. */
        pthread_mutex_lock (&store->mutexCapteur);
        printJSON(response, &store->capteur);
        pthread_mutex_unlock (&store->mutexCapteur);
        /* Fin de la zone protegee. */
    }
    else if (strcmp(params[0], "CAPT_JSON_INTERVAL") == 0) {
        char json[LBUF];
        clock_t timer;
        unsigned int frequence;
        while (1) {
            timer = clock();
            /* Debut de la zone protegee. */
            pthread_mutex_lock (&store->mutexCapteur);
            printJSON(json, &store->capteur);
            frequence = store->frequence;
            pthread_mutex_unlock (&store->mutexCapteur);
            /* Fin de la zone protegee. */
            sprintf(response, "<start|%s|end>", json);
            if (write(sid,response, strlen(response)) < 0) {
                close(sid);
                perror("writeResponceInterval");
                return;
            }
            fprintf(stderr, "Emit Interval : %s\n", response);
            usleep(frequence * 1000 - (clock()-timer));
        }
    }
    else if (strcmp(params[0], "CH_FREQ") == 0 && nbParams == 2) {
        /* Debut de la zone protegee. */
        pthread_mutex_lock (&store->mutexCapteur);
        if (strtol(params[1], NULL, 0) >= 100) {
            store->frequence = (unsigned int) strtol(params[1], NULL, 0);
            sprintf(response, "La frequence est maintenant de %d", store->frequence);
        } else
            sprintf(response, "Valeur incorrecte, la frequence est de %d", store->frequence);
        pthread_mutex_unlock (&store->mutexCapteur);
        /* Fin de la zone protegee. */
    }
    else if (strcmp(params[0], "QUIT") == 0) {
        exit(42);
    }
    else {
        sprintf(response, "ERROR : Bad request !");
    }
    if (write(sid,response, strlen(response)) < 0) {
        close(sid);
        perror("writeResponce");
        return;
    }
    fprintf(stderr, "Emit : %s\n", response);
}
int splitParams(char *request, char **params)
{
    int i = 0;
    while (1) {
        // On recherche le debut du mot
        while (*request != '\0') {
		    if (!isSepa(*request))
                break;
		    request++;
	    }
        if (*request == '\0') // Si aucun debut de mot trouve
            break;
        params[i++]=request;
        // On recherche la fin du mot
        while (*request != '\0') {
            if (isSepa(*request))
                break;
            request++;
        }
        if (*request == '\0') // Si c'est la fin de la chaine
            break;
        *request = '\0'; // On delimite la fin du mot
        request++;
    }
    params[i] = NULL;
    for(i=0; params[i] != NULL; i++)
    {
        printf("%d: %s\n", i, params[i]);
    }
    return i;
}

/* fonction qui determine si un caractere est un separateur */
int isSepa(char c)
{
	if (c == ' ') return 1;
	if (c == '\t') return 1;
	return 0;
}
