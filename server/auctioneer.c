/*
 * Module de gestion de la mise en vente des objets (a.k.a commissaire)
 */
#include <sys/time.h>

#include "pse.h"
#include "defs.h"
#include "auctioneer.h"

void *createAuctioneer () {
	int ret;
	int lastBidder[TMAX];
	
	memset(lastBidder, -1, sizeof(lastBidder));
	printf("commissaire: lancement de la vente des %d objets choisis\n", nbObjs);
	if (pthread_mutex_lock (&mutexBid) != 0) {
		erreur_IO ("mutex_lock");
	}
	while (objInSale < nbObjs) {
		if (lastBidder[objInSale] == -1) {
			objs[objInSale].prix_cur = objs[objInSale].prix_ini;
		}
		printf("commissaire: mise en vente de l'objet %s à %f\n", objs[objInSale].nom, objs[objInSale].prix_cur);
		ret = takeBid();
		if (ret == 0) {
			objs[objInSale].prix_cur = bid;
			lastBidder[objInSale] = bidder;
			printf("commissaire: nouvelle enchere : %f\n", objs[objInSale].prix_cur);
		}
		else if (ret == ETIMEDOUT) {
			if (lastBidder[objInSale] == -1)
				printf("commissaire: pas d'acheteur pour l'objet %d\n", objInSale);
			else
				printf("commissaire: objet %d vendu pour %f au client du worker %d\n", objInSale, objs[objInSale].prix_cur, lastBidder[objInSale]);
			state++;
			objInSale++;
			if (pthread_cond_broadcast(&condBid) != 0) {
				erreur_IO("pthread_cond_broadcast");
			}
		}
		else {
			fprintf(stderr, "pthread_cond_timedwait: %d", ret);
		}
	}
	printf("commissaire: fin de la vente\n");
	state = -1;
	if (pthread_mutex_unlock (&mutexBid) != 0) {
		erreur_IO ("mutex_unlock");
	}
	
	pthread_exit(NULL);
}

int takeBid () {
	int lastState = state, ret = 0;
	struct timespec timeToWait;
	struct timeval now;
	
	gettimeofday(&now,NULL);
	timeToWait.tv_sec = now.tv_sec + 20;
	timeToWait.tv_nsec = now.tv_usec * 1000;
	
	while (ret == 0 && state == lastState) {
		ret = pthread_cond_timedwait(&condBid, &mutexBid, &timeToWait);
	}
	return ret;
}
