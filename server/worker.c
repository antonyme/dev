/*
 * Module de gestion des workers (threads individuels de gestion de client).
 */
#include "pse.h"
#include "defs.h"
#include "worker.h"
#include "clientIO.h"

void *traiterRequete (void *arg) {
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
		sendCli(data->canal, "Welcome to our auction %s!", buf);
		recvCli(data->canal, buf);
		
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
