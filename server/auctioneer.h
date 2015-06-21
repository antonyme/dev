#ifndef AUCTIONEER_H
#define AUCTIONEER_H

/*
 * Crée le commissaire priseur de la vente, il est chargé d'annoncer les objets
 * mis en vente  et de gérer les offres
 */
void *createAuctioneer ();

/*
 * Attend une enchère ou l'expiration du délais d'attente
 */
int waitBid ();

/*
 * Poste sur le semaphore des workers actifs 
 */
void wakeClients ();

#endif
