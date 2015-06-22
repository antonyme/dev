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
	int sd, activity, stay = VRAI, lastBid = FAUX;
	char buf[LIGNE_MAX];
	ACHETEUR myInfos;
	OBJET toBuy;
	fd_set readfds;
	struct timeval tv;
	
	signal(SIGPIPE, SIG_IGN);
	srand(time(NULL));
	
	if (argc != 4) {
		erreur("usage: %s machine port clientName\n", argv[0]);
	}
	
	printf("%s: retrieving info about client\n", CMD);
	fillInfos(&myInfos, argv[3]);
	myInfos.argent_cur = myInfos.argent_ini;

	printf("%s: joining auction\n", CMD);
	sd = connectToServ(argv[1], argv[2]);
	sendServ(sd, myInfos.nom);
	
	while (stay) {
		
		//recv object
		recvServ(sd, buf);
		sscanf(buf+2, "%s %f %f %c %d", toBuy.nom, &toBuy.prix_ini, &toBuy.prix_cur, &toBuy.type, &toBuy.rare);
		printf("%s: objet %s mis en vente au prix %f\n", CMD, toBuy.nom, toBuy.prix_cur);
		
		while (VRAI) {
			
			//clear the socket set
			FD_ZERO(&readfds);
			//add socket to set
			FD_SET(sd, &readfds);
			
			//set hang time
			if (lastBid) { //no double bid
				myInfos.latence = LATMAX;
			}
			else getHangTime(&myInfos, &toBuy);
			tv.tv_sec = (long)myInfos.latence;
			tv.tv_usec = 0;
			if (tv.tv_sec == LATMAX)
				if (lastBid) {
					printf("%s: meilleur offre: attente\n", CMD);
				}
				else {
					printf("%s: non interesse\n", CMD);
				}
			else
				printf("%s: decide de monter a %f au bout de %f secondes\n", CMD, myInfos.prix_prop, myInfos.latence);

			//wait
			activity = select(sd+1, &readfds, NULL, NULL, &tv);
			if(activity < 0) {
				erreur_IO("select");
			}
			else if (activity > 0) { //server message
				recvServ(sd, buf);
			}
			else { //select timed out
				sendServ(sd, "b %f %f", myInfos.prix_prop, toBuy.prix_cur);
				recvServ(sd, buf);
				if (strcmp(buf, "accepted") == 0) {
					printf("%s: enchere accepte\n", CMD);
					toBuy.prix_cur = myInfos.prix_prop;
					lastBid = VRAI;
					continue;
				}
				else {
					printf("%s: enchere refuse\n", CMD);
				}
			}
			
			//process server message
			if (strcmp(buf, "end object") == 0) {
				break;
			}
			else if (strcmp(buf, "end") == 0) {
				stay = FAUX;
				break;
			}
			else if (buf[0] == 'n') { //new price
				sscanf(buf+2, "%f", &toBuy.prix_cur);
				lastBid = FAUX;
				printf("%s: l'objet %s est monte au prix %f\n", CMD, toBuy.nom, toBuy.prix_cur);
			}
			else {
				erreur("wrong message from server: %s\n", buf);
			}
		}
		
		if (lastBid) {
			lastBid = FAUX;
			printf("%s: objet %s obtenu au prix %f\n", CMD, toBuy.nom, toBuy.prix_cur);
		}
	}
	
	if (close(sd) == -1) {
		erreur_IO("close");
	}
	
	exit(EXIT_SUCCESS);
}
