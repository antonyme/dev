#ifndef AUCTIONEER_H
#define AUCTIONEER_H

/*
 * Crée le commissaire priseur de la vente, il est chargé d'annoncer les objets
 * mis en vente  et de gérer les offres
 */
void *createAuctioneer ();

void setNewState (int numObj, float price);

int takeBid ();

#endif
