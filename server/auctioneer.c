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
	
	printf("Lancement de la vente des %d objets choisis\n", nbObjs);
	if (pthread_mutex_lock (&mutexBid) != 0) {
		erreur_IO ("mutex_lock");
	}
	while (numObj != nbObjs - 1) {
		setNewState(numObj, price);
		ret = takeBid();
		if (ret == 0) {
			price = bid;	
		}
		else if (ret == ETIMEDOUT) {
			printf("Objet %d vendu\n", numObj);
			numObj++;
			price = objs[numObj].prix_ini;
		}
		else {
			fprintf(stderr, "pthread_cond_timedwait: %d", ret);
		}
	}
	pthread_exit(NULL);
}

void setNewState (int numObj, float price) {
	if (pthread_mutex_lock (&mutexObjs) != 0) {
		erreur_IO ("mutex_lock");
	}
	
	objInSale = numObj;
	objs[numObj].prix_cur = price;
	
	if (pthread_mutex_unlock (&mutexObjs) != 0) {
		erreur_IO ("mutex_unlock");
	}
}

int takeBid () {
	struct timespec timeToWait;
	struct timeval now;
	
	gettimeofday(&now,NULL);
	timeToWait.tv_sec = now.tv_sec+20;
	timeToWait.tv_nsec = now.tv_usec * 1000;
	
	return pthread_cond_timedwait(&condBid, &mutexBid, &timeToWait);
}
