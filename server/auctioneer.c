/*
 * Module de gestion de la mise en vente des objets
 */
#include <sys/time.h>

#include "pse.h"
#include "defs.h"
#include "auctioneer.h"

void *createAuctioneer () {
	int numObj = 0, ret;
	float price = objs[numObj].prix_ini;
	
	printf("commissaire: lancement de la vente des %d objets choisis\n", nbObjs);
	if (pthread_mutex_lock (&mutexBid) != 0) {
		erreur_IO ("mutex_lock");
	}
	while (numObj < nbObjs) {
		objInSale = numObj;
		objs[numObj].prix_cur = price;
		printf("commissaire: mise en vente de l'objet %s à %f\n", objs[numObj].nom, objs[numObj].prix_cur);
		ret = takeBid();
		if (ret == 0) {
			price = bid;
			printf("commissaire: nouvelle enchere : %f\n", price);
		}
		else if (ret == ETIMEDOUT) {
			state++;
			printf("commissaire: objet %d vendu pour %f = %f\n", numObj, price, bid);
			numObj++;
			price = objs[numObj].prix_ini;
			pthread_cond_broadcast(&condBid);
		}
		else {
			fprintf(stderr, "pthread_cond_timedwait: %d", ret);
		}
	}
	printf("commissaire: fin de la vente\n");
	pthread_exit(NULL);
}

void setNewState (int numObj, float price) {

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
