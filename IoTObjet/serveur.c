#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "serveur.h"
#include "capteur.h"

#define LBUF 1024

int analyseRequest(char *request, char *response, Store *store, int sid)
{
    char* params[255];
    int nbParams = splitParams(request, params);
    if (nbParams == 0) {
        sprintf(response, "ERROR : No request !");
        return 0;
    }
    if (strcmp(params[0], "CAPT_JSON") == 0) {
        /* Debut de la zone protegee. */
        pthread_mutex_lock (&store->mutexCapteur);
        printJSON(response, &store->capteur);
        pthread_mutex_unlock (&store->mutexCapteur);
        /* Fin de la zone protegee. */
    }
    else if (strcmp(params[0], "CAPT_JSON_INTERVAL") == 0) {
        return 1;
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
    return 0;
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
