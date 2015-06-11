#include "pse.h"
#include "defs.h"
#include "main.h"
#include "servInt.h"
#include "auction.h"
#include "fileIO.h"

int main(int argc, char *argv[]) {
	int sd, arret = FAUX, ret;
	char buf[LIGNE_MAX];
	ACHETEUR myInfos;
	
	signal(SIGPIPE, SIG_IGN);
	
	if (argc != 4) {
		erreur("usage: %s machine port clientName\n", argv[0]);
	}
	sd = connectToServ(argv[1], argv[2]);
	fillInfos(&myInfos, argv[3]);
	
	
	while (arret == FAUX) {
		printf("ligne> ");
		if (fgets(buf, LIGNE_MAX, stdin) == NULL) {
			printf("Fin de fichier (ou erreur) : arret.\n");
			arret = VRAI;
			continue;
		}
		else {
			ret = ecrireLigne(sd, buf);
			if (ret == -1) {
				erreur_IO("ecrireLigne");
			}
			if (strcmp(buf, "fin\n") == 0) {
				printf("%s: arret demande.\n", CMD);
				arret = VRAI;
			}
			else {
				printf("%s: ligne de %d octets envoyee au serveur.\n", CMD, ret);
			}
		}
	}
	
	if (close(sd) == -1) {
		erreur_IO("close");
	}
	
	exit(EXIT_SUCCESS);
}

