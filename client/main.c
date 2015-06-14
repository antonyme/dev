/*
 * Client - main
 */
#include "pse.h"
#include "defs.h"
#include "main.h"
#include "servIO.h"
#include "auction.h"
#include "fileIO.h"

int main(int argc, char *argv[]) {
	int sd;
	char buf[LIGNE_MAX];
	ACHETEUR myInfos;
	
	signal(SIGPIPE, SIG_IGN);
	
	if (argc != 4) {
		erreur("usage: %s machine port clientName\n", argv[0]);
	}
	sd = connectToServ(argv[1], argv[2]);
	printf("%s: retrieving info about client\n", CMD);
	fillInfos(&myInfos, argv[3]);
	printf("%s: joining auction\n", CMD);
	sendServ(sd, myInfos.nom);
	recvServ(sd, buf);
	scanf("%s", buf);
	sendServ(sd, buf);
	
	if (close(sd) == -1) {
		erreur_IO("close");
	}
	
	exit(EXIT_SUCCESS);
}
