/*
 * Server - main
 */
#include "pse.h"
#include "defs.h"
#include "clientIO.h"
#include "worker.h"

int main(int argc, char *argv[]) {
	int ecoute, canal, i;
	struct sockaddr_in reception;
	socklen_t receptionlen = sizeof(reception);
	
	DataSpec cohorte[NTHREADS];
	
	if (argc != 2) {
		erreur("usage: %s port\n", argv[0]);
	}
	createCohorte(cohorte);
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
		
		i = NTHREADS;
		while (i == NTHREADS) {
			for (i=0; i<NTHREADS; i++)
				if (cohorte[i].libre) break;
			if (i == NTHREADS) usleep(ATTENTE);
		}
		
		cohorte[i].canal = canal;
		
		printf("%s: worker %d choisi\n", CMD, i);
		
	}
	
	exit(EXIT_SUCCESS);
}