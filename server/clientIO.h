#ifndef CLIENTIO_H
#define CLIENTIO_H

/*
 * Ouvre un canal d'écoute sur le port (strPort) et retourne le descripteur
 * du canal.
 */
int createEcoute (char strPort[]);

/*
 * Envoi au client la string crée à partir du format et de la liste d'arguments.
 * Attend que le client réponde "OK".
 */
void sendCli (int sd, const char* format, ...);

/*
 * Attend et reçoit une ligne envoyé par le client.
 * Répond "OK" si pas d'erreur.
 */
void recvCli (int sd, char* buf);

/*
 * Test si une lecture sur sd serait bloquante. Pour cela lit le premier charctère et
 * le place en buf[0]
 */
int readWouldBlock (int sd, char* buf);

#endif
