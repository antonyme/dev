/*
 * Module de gestion des workers (threads individuels de gestion de client).
 */
#include <sys/time.h>

#include "pse.h"
#include "defs.h"
#include "worker.h"
#include "clientIO.h"

void *traiterRequete (void *arg) {
	int i, ret, stay = VRAI, lastPrice;
	float prix_prop;
	DataSpec * data = (DataSpec *) arg;
	char buf[LIGNE_MAX];
	
	while (VRAI) {
		printf("worker %d: wait canal.\n", data->tid);
		if (sem_wait(&data->sem) == -1) {
			erreur_IO("sem_wait");
		}
		data->libre = FAUX;
		
		//recv infos
		recvCli(data->canal, buf);
		printf("worker %d: new client (%s) on canal %d.\n", data->tid, buf, data->canal);
		
		//join
		nbClients++;
		for(i = 0; i<NTHREADS; i++)
			if(clients[i] == -1) break;
		clients[i] = data->tid;
		
		printf("worker %d: wait auction start.\n", data->tid);
		if (sem_wait(&data->sem) == -1) {
			erreur_IO("sem_wait");
		}
		
		while (stay) {		
			//barrier
			ret = pthread_barrier_wait(&auctionStart);
			if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
				erreur_IO ("barrier_wait");
			}
			
			//send object
			printf("worker %d: send object infos: %s\n", data->tid, curObj->nom);
			sendCli(data->canal, "o %s %f %f %c %d", curObj->nom, curObj->prix_ini, curObj->prix_cur, curObj->type, curObj->rare);
			lastPrice = curObj->prix_ini;
			
			while(VRAI) {
				
				//wait for a change
				if (sem_wait(&data->sem) == -1) {
					erreur_IO("sem_post");
				}
printf("worker %d: woke\n", data->tid);
				//lock bid
				if (pthread_mutex_lock (&mutexBid) != 0) {
					erreur_IO ("mutex_lock");
				}
printf("worker %d: lock\n", data->tid);
				//woke by auctioneer
				if (end) {
					sendCli(data->canal, "end");
					stay = FAUX;
					break;
				}
				if (endObj) {
					sendCli(data->canal, "end object");
					break;
				}
				if (bid != lastPrice) { //new price
printf("worker %d: send new price : %f\n", data->tid, bid);
					lastPrice = bid;
					sendCli(data->canal, "n %f", bid);
				}
				
				//woke by server
				if (clientMessage) {
					clientMessage = FAUX;
					recvCli(data->canal, buf);
printf("worker %d: client bid: %s\n", data->tid, buf);
					if (buf[0] == 'b') { //new bid
						sscanf(buf+2, "%f", &prix_prop);
printf("worker %d: client bid: %f bid: %f prix cur: %f\n", data->tid, prix_prop, bid, curObj->prix_cur);
						if (bid == curObj->prix_cur && prix_prop > bid) { //no previous bet unprocessed
							bid = prix_prop;
							bidder = data->tid;
							sendCli(data->canal, "accepted");
							
							//wake auctioneer
							pthread_cond_signal(&condBid);
						}
					}
				}
				
				//unlock bid
				if (pthread_mutex_unlock (&mutexBid) != 0) {
					erreur_IO ("mutex_unlock");
				}
			}
		}
		
		sendCli(data->canal, "f");	
		if (close(data->canal) == -1) {
			erreur_IO("close");
		}
		data->canal = -1;
		data->libre = VRAI;
		if (sem_post(&semWork) == -1) {
			erreur_pthread_IO("sem_post");
		}
	}
	pthread_exit(NULL);
}

void createCohorte () {
	int ret, i;
	
	for (i=0; i<NTHREADS; i++) {
		cohorte[i].tid = i;
		cohorte[i].libre = VRAI;
		
		//-1 -> no client assigned
		cohorte[i].canal = -1;
		
		//init sem
		if (sem_init(&cohorte[i].sem, 0, 0) == -1) {
			erreur_IO("sem_init");
		}
		
		//create thread
		ret = pthread_create(&cohorte[i].id, NULL, traiterRequete, &cohorte[i]);
		if (ret != 0) {
			erreur_IO("pthread_create");
		}
	}
}