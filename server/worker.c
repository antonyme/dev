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
		while (data->canal == -1) {
			usleep(ATTENTE);
		}
		data->libre = FAUX;
		printf("worker %d: working on canal %d.\n", data->tid, data->canal);
		recvCli(data->canal, buf);
		sendCli(data->canal, "Welcome to our auction %s!", buf);
		
		if (close(data->canal) == -1) {
			erreur_IO("close");
		}
		data->canal = -1;
		data->libre = VRAI;
	}
	
	pthread_exit(NULL);
}

void createCohorte (DataSpec cohorte[]) {
	int ret, i;
	
	for (i=0; i<NTHREADS; i++) {
		cohorte[i].tid = i;
		cohorte[i].libre = VRAI;
		/* une valeur -1 indique pas de requete a traiter */
		cohorte[i].canal = -1;
		ret = pthread_create(&cohorte[i].id, NULL, traiterRequete, &cohorte[i]);
		if (ret != 0) {
			erreur_IO("pthread_create");
		}
	}
}
