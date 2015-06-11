#ifndef SERVINT_H
#define SERVINT_H

/*
 * Réalise toutes les étapes nécessaire à la mise en place d'une connection
 * avec le serveur sur machine écoutant sur port.
 * Retourne le numéro de la socket connecté.
 */
int connectToServ(char machine[], char port[]);

#endif