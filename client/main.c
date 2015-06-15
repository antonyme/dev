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
	int sd, stay = VRAI;
	char buf[LIGNE_MAX];
	ACHETEUR myInfos;
	OBJET toBuy;
	
	signal(SIGPIPE, SIG_IGN);
	srand(time(NULL));
	
	if (argc != 4) {
		erreur("usage: %s machine port clientName\n", argv[0]);
	}
	printf("%s: retrieving info about client\n", CMD);
	fillInfos(&myInfos, argv[3]);
	myInfos.argent_cur = myInfos.argent_ini;
	sd = connectToServ(argv[1], argv[2]);
	printf("%s: joining auction\n", CMD);
	sendServ(sd, myInfos.nom);
	while (stay) {
		recvServ(sd, buf);
		switch (buf[0]) {
		case 'o' :		//new object
			sscanf(buf+2, "%s %f %f %c %d", toBuy.nom, &toBuy.prix_ini, &toBuy.prix_cur, &toBuy.type, &toBuy.rare);
			printf("%s: objet %s mis en vente au prix %f\n", CMD, toBuy.nom, toBuy.prix_cur);
			break;
		case 'p' :		//new price
			sscanf(buf+2, "%f", &toBuy.prix_cur);
			printf("%s: l'objet %s est monte au prix %f\n", CMD, toBuy.nom, toBuy.prix_cur);
			break;
		default :
			erreur("message du serveur inattendu : %s", buf);
			break;
		}
		if(buf[0] == 'o' || buf[0] == 'p') { 	//decide how to act
			getHangTime(&myInfos, &toBuy);
			sendServ(sd, "%f %f", myInfos.latence, myInfos.prix_prop);
			if(myInfos.latence == LATMAX)
				printf("%s: non interesse\n", CMD);
			else
				printf("%s: decide de monter a %f au bout de %f secondes\n", CMD, myInfos.prix_prop, myInfos.latence);
		}
	}
	
	if (close(sd) == -1) {
		erreur_IO("close");
	}
	
	exit(EXIT_SUCCESS);
}
