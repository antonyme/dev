#ifndef SERVIO_H
#define SERVIO_H

/*
 * Réalise toutes les étapes nécessaire à la mise en place d'une connection
 * avec le serveur sur machine écoutant sur port.
 * Retourne le numéro de la socket connecté.
 */
int connectToServ (char machine[], char port[]);

/*
 * Envoi au serveur la string crée à partir du format et de la liste d'arguments.
 * Attend que le serveur réponde "OK".
 */
void sendServ (int sd, const char* format, ...);

/*
 * Attend et reçoit une ligne envoyé par le server.
 * Répond "OK" si pas d'erreur.
 */
void recvServ (int sd, char* buf);

#endif