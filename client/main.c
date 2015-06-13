#include "pse.h"
#include "defs.h"
#include "main.h"
#include "servIO.h"
#include "auction.h"
#include "fileIO.h"

int main(int argc, char *argv[]) {
	int sd;
	ACHETEUR myInfos;
	
	signal(SIGPIPE, SIG_IGN);
	
	if (argc != 4) {
		erreur("usage: %s machine port clientName\n", argv[0]);
	}
	sd = connectToServ(argv[1], argv[2]);
	fillInfos(&myInfos, argv[3]);
	sendServ(sd, myInfos.nom);
	
	if (close(sd) == -1) {
		erreur_IO("close");
	}
	
	exit(EXIT_SUCCESS);
}

