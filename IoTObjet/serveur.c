#include <stdio.h>

#include "serveur.h"
#include "capteur.h"

void analyseRequest(char *request, char *response, Store *store)
{
    char* params[255];
    int nbParams = splitParams(request, params);
    if (nbParams == 0) {
        sprintf(response, "ERROR : No request !");
        return;
    }
    if (strcmp(params[0], "CAPT_JSON") == 0) {
        printJSON(response, &store->capteur);
    }
    else {
        sprintf(response, "ERROR : Bad request !");
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
