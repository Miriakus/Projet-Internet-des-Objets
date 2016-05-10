#include <stdio.h>

#include "serveur.h"
#include "capteur.h"

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
        while (1) {
            /* Debut de la zone protegee. */
            pthread_mutex_lock (&store->mutexCapteur);
            printJSON(response, &store->capteur);
            pthread_mutex_unlock (&store->mutexCapteur);
            /* Fin de la zone protegee. */
            if (write(sid,response, strlen(response)) < 0) {
                close(sid);
                perror("writeResponceInterval");
                return;
            }
            fprintf(stderr, "Emit Interval : %s\n", response);
            sleep(1);
        }
    }
    else {
        sprintf(response, "ERROR : Bad request !");
    }
    if (write(sid,response, strlen(response)) < 0) {
        close(sid);
        perror("writeResponce");
        return NULL;
    }
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
