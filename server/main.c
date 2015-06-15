/*
 * Server - main
 */
#include "pse.h"
#include "defs.h"
#include "clientIO.h"
#include "worker.h"
#include "auctioneer.h"
#include "fileIO.h"

pthread_mutex_t mutexObjs = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condBid = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexBid = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	int ecoute, canal, i, ret;
	pthread_t auctId = NTHREADS;
	struct sockaddr_in reception;
	socklen_t receptionlen = sizeof(reception);
	
	if (argc != 2) {
		erreur("usage: %s port\n", argv[0]);
	}
	if (sem_init(&sem_work, 0, NTHREADS) == -1) {
		erreur_IO("sem_init");
	}
	state = 0;
	
	fillAllObjs();
	createCohorte();
	ret = pthread_create(&auctId, NULL, createAuctioneer, NULL);
	if (ret != 0) {
		erreur_IO("pthread_create");
	}
	ecoute = createEcoute(argv[1]);
	
	while (VRAI) {
		printf("%s: waiting to a connection\n", CMD);
		canal = accept(ecoute, (struct sockaddr *) &reception, &receptionlen);
		if (canal < 0) {
			erreur_IO("accept");
		}
		printf("%s: adress: %s, port: %hu\n", CMD,
				stringIP(ntohl(reception.sin_addr.s_addr)),
				ntohs(reception.sin_port));
		
		if (sem_wait(&sem_work) == -1) {
			erreur_IO("sem_wait");
		}
		for (i=0; i<NTHREADS; i++)
			if (cohorte[i].libre) break;
		
		cohorte[i].canal = canal;
		if (sem_post(&cohorte[i].sem) == -1) {
			erreur_IO("sem_post");
		}
		
		printf("%s: worker %d choisi\n", CMD, i);
		
	}
	
	exit(EXIT_SUCCESS);
}