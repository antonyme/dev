/*
* Server - main
*/
#include "pse.h"
#include "defs.h"
#include "main.h"
#include "clientIO.h"
#include "worker.h"
#include "auctioneer.h"
#include "fileIO.h"

pthread_cond_t condBid = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexBid = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	int masterSd, newSd, maxSd, activity, i, ret;
	pthread_t auctId = NTHREADS;
	struct sockaddr_in reception;
	socklen_t receptionlen = sizeof(reception);
	fd_set readfds;
	
	if (argc != 2) {
		erreur("usage: %s port\n", argv[0]);
	}
	
	init();
	fillAllObjs();
	createCohorte();
	if ((ret = pthread_create(&auctId, NULL, createAuctioneer, NULL)) != 0) {
		erreur_IO("pthread_create");
	}
	masterSd = createEcoute(argv[1]);
	
	while (VRAI) {
		
		//clear the socket set
		FD_ZERO(&readfds);
		
		//add master socket to set
		FD_SET(masterSd, &readfds);
		maxSd = masterSd;
		
		//add child sockets to set
		for (i = 0; i < NTHREADS; i++) {
			//if valid socket descriptor then add to read list
			if(!cohorte[i].libre)
				FD_SET(cohorte[i].canal, &readfds);
			
			//highest file descriptor number, need it for the select function
			if(cohorte[i].canal > maxSd)
				maxSd = cohorte[i].canal;
		}
		
		//wait indefinitely for an activity on one of the sockets
		activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);
		if (activity <= 0) {
			erreur_IO("select");
		}
		
		//if something happened on the master socket , then its an incoming connection
		if (FD_ISSET(masterSd, &readfds)) {
			
			printf("%s: accepting new connection\n", CMD);
			newSd = accept(masterSd, (struct sockaddr *) &reception, &receptionlen);
			if (newSd < 0) {
				erreur_IO("accept");
			}
			printf("%s: adress: %s, port: %hu\n", CMD,
					stringIP(ntohl(reception.sin_addr.s_addr)),
					ntohs(reception.sin_port));
			
			if (sem_wait(&semWork) == -1) {
				erreur_IO("sem_wait");
			}
			
			//find free worker
			for (i=0; i<NTHREADS; i++)
				if (cohorte[i].libre) break;
			
			//set canal
			cohorte[i].canal = newSd;
			
			//wake worker
			if (sem_post(&cohorte[i].sem) == -1) {
				erreur_IO("sem_post");
			}
			
			printf("%s: worker %d choisi\n", CMD, i);
		}
		else { //else it's IO on an other socket
			clientMessage = VRAI;
			for(i = 0; i<NTHREADS; i++) {
				if(FD_ISSET(cohorte[i].canal, &readfds)) {
					sem_getvalue(&cohorte[i].sem, &ret);
					if(ret < 1) {
						//wake worker for IO
						if (sem_post(&cohorte[i].sem) == -1) {
							erreur_IO("sem_post");
						}	
					}
				}
			}	
		}
	}
	
	exit(EXIT_SUCCESS);
}

void init () {
	clientMessage = endObj = end = FAUX;
	
	nbClients = 0;
	memset(clients, -1, sizeof(clients));
	
	nbObjs = 0;
	memset(objs, 0, sizeof(objs));
	
	bid = 0;
	bidder = -1;
	
	if (sem_init(&semWork, 0, NTHREADS) == -1) {
		erreur_IO("sem_init");
	}
}
