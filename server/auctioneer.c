/*
 * Module de gestion de la mise en vente des objets (a.k.a commissaire)
 */
#include <sys/time.h>

#include "pse.h"
#include "defs.h"
#include "auctioneer.h"

void *createAuctioneer () {
	int i, ret, lastBidder[TMAX];
	
	memset(lastBidder, -1, sizeof(lastBidder));
	sleep(15);
	printf("commissaire: lancement de la vente des %d objets choisis\n", nbObjs);
	
	//init barrier
	if (pthread_barrier_init(&auctionStart, NULL, nbClients + 1) != 0) {
		erreur_IO ("barrier_init");
	}
	
	for (i = 0; i<nbObjs; i++) {
		wakeClients();
		
		//set object
		curObj = objs[i];
		
		bid = curObj.prix_cur = curObj.prix_ini;
		
		//barrier (wait clients)
		ret = pthread_barrier_wait(&auctionStart);
		if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
			erreur_IO ("barrier_wait");
		}
		
		//lock bid
		if (pthread_mutex_lock (&mutexBid) != 0) {
			erreur_IO ("mutex_lock");
		}
		
		endObj = FAUX;
		
		while (VRAI) {
			printf("commissaire: mise en vente de l'objet %s à %f\n", curObj.nom, curObj.prix_cur);
			ret = waitBid();
			if (ret == 0) { //signaled (bid from client)
				curObj.prix_cur = bid;
				lastBidder[i] = bidder;
			}
			else if (ret == ETIMEDOUT) { //timed out (no new bid)
				if (lastBidder[i] == -1)
					printf("commissaire: pas d'acheteur pour l'objet %s\n", curObj.nom);
				else
					printf("commissaire: objet %s vendu pour %f au client du worker %d\n", curObj.nom, curObj.prix_cur, lastBidder[i]);
				endObj = VRAI;
				break;
			}
			else {
				fprintf(stderr, "pthread_cond_timedwait: %d", ret);
			}
			wakeClients();
		}
	}
	end = VRAI;
	printf("commissaire: fin de la vente\n");
	if (pthread_mutex_unlock (&mutexBid) != 0) {
		erreur_IO ("mutex_unlock");
	}
	
	pthread_exit(NULL);
}

int waitBid () {
	int ret = 0;
	struct timespec timeToWait;
	struct timeval now;
	
	gettimeofday(&now,NULL);
	timeToWait.tv_sec = now.tv_sec + 20;
	timeToWait.tv_nsec = now.tv_usec * 1000;
	
	while (ret == 0 && bid == curObj.prix_cur) {
		ret = pthread_cond_timedwait(&condBid, &mutexBid, &timeToWait);
	}
	return ret;
}

void wakeClients () {
	int i;
	
	for (i = 0; i<nbClients; i++) {
		if (sem_post(&cohorte[clients[i]].sem) == -1) {
			erreur_IO("sem_post");
		}
	}
}
