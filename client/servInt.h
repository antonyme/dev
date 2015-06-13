#ifndef SERVINT_H
#define SERVINT_H

/*
 * Réalise toutes les étapes nécessaire à la mise en place d'une connection
 * avec le serveur sur machine écoutant sur port.
 * Retourne le numéro de la socket connecté.
 */
int connectToServ (char machine[], char port[]);

/*
 * Attend que le serveur réponde "OK".
 * Retourne 0 en cas de succes, -1 sinon.
 */
int waitACK (int sd);

/*
 * Envoi au serveur le nom du client et attend acceptation dans la salle de 
 * vente
 */
void joinRoom (int sd, ACHETEUR* info);

#endif