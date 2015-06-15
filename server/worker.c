/*
 * Module de gestion des workers (threads individuels de gestion de client).
 */
#include <sys/time.h>

#include "pse.h"
#include "defs.h"
#include "worker.h"
#include "clientIO.h"

void *traiterRequete (void *arg) {
	int stay = VRAI, numObj, ret;
	float latence, prix_prop;
	DataSpec * data = (DataSpec *) arg;
	char buf[LIGNE_MAX];
	
	while (VRAI) {
		printf("worker %d: wait canal.\n", data->tid);
		if (sem_wait(&data->sem) == -1) {
			erreur_IO("sem_post");
		}
		data->libre = FAUX;
		recvCli(data->canal, buf);
		printf("worker %d: new client (%s) on canal %d.\n", data->tid, buf, data->canal);
		
		while (stay) {
			numObj = objInSale;
			printf("worker %d: envoi d'infos sur l'objet %s\n", data->tid, objs[numObj].nom);
			sendCli(data->canal, "o %s %f %f %c %d", objs[numObj].nom, objs[numObj].prix_ini, objs[numObj].prix_cur, objs[numObj].type, objs[numObj].rare);
			recvCli(data->canal, buf);
			sscanf(buf, "%f %f", &latence, &prix_prop);
			while(numObj == objInSale) {
				printf("worker %d: attente %f avant proposition a %f\n", data->tid, latence, prix_prop);
				ret = waitToBid(latence);
				if (ret == 0) {	//trop lent à réagir
					pthread_mutex_unlock(&mutexBid);
					printf("worker %d: reveille avant fin d'attente\n", data->tid);
					if (numObj == objInSale) {
						printf("worker %d: envoi du nouveau prix\n", data->tid);
						sendCli(data->canal, "p %f", objs[objInSale].prix_cur);
						recvCli(data->canal, buf);
						sscanf(buf, "%f %f", &latence, &prix_prop);
					}
					else break;
				}
				else {		//fin de latence
					state++;
					bid = prix_prop;
					latence = LATMAX; //pas d'autosurenchere
					pthread_cond_broadcast(&condBid);
					pthread_mutex_unlock(&mutexBid);
				}	
			}
		}
		
		if (close(data->canal) == -1) {
			erreur_IO("close");
		}
		data->canal = -1;
		data->libre = VRAI;
		if (sem_post(&sem_work) == -1) {
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
		/* une valeur -1 indique pas de requete a traiter */
		cohorte[i].canal = -1;
		if (sem_init(&cohorte[i].sem, 0, 0) == -1) {
			erreur_IO("sem_init");
		}
		ret = pthread_create(&cohorte[i].id, NULL, traiterRequete, &cohorte[i]);
		if (ret != 0) {
			erreur_IO("pthread_create");
		}
	}
}

int waitToBid (float tmp) {
	int lastState = state, ret = 0;
	struct timespec timeToWait;
	struct timeval now;
	
	gettimeofday(&now,NULL);
	timeToWait.tv_sec = now.tv_sec + (int)tmp;
	timeToWait.tv_nsec = now.tv_usec * 1000;
	
	if (pthread_mutex_lock (&mutexBid) != 0) {
		erreur_IO ("mutex_lock");
	}
	while (ret == 0 && state == lastState) {
		ret = pthread_cond_timedwait(&condBid, &mutexBid, &timeToWait);
	}
	return ret;
}
