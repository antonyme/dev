/* 
 * Module de gestion de la connection au serveur et de la
 * communication avec celui-ci.
 */
#include "pse.h"
#include "defs.h"
#include "servIO.h"

int connectToServ(char machine[], char port[]) {
	struct sockaddr_in *sa;
	int sd, ret;
	
	printf("%s: creating a socket\n", CMD);
	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		erreur_IO("socket");
	}
	
	printf("%s: DNS resolving for %s, port %s\n", CMD, machine, port);
	sa = resolv(machine, port);
	if (sa == NULL) {
		erreur("adresse %s port %s inconnus\n", machine, port);
	}
	printf("%s: adr %s, port %hu\n", CMD,
			stringIP(ntohl(sa->sin_addr.s_addr)),
			ntohs(sa->sin_port));
	
	printf("%s: connecting the socket\n", CMD);
	ret = connect(sd, (struct sockaddr *) sa, sizeof(struct sockaddr_in));
	if (ret < 0) {
		erreur_IO("Connect");
	}

	freeResolv();
	return sd;
}

void sendServ (int sd, const char* line) {
	int ret;
	char buf[LIGNE_MAX];
	
	strcpy(buf, line);
	ret = ecrireLigne(sd, buf);
	if (ret == -1) {
		erreur_IO("sendServ - ecrireLigne");
	}
	printf("\tsent: %s\n\t%d octets\n", CMD, line, ret);

	while ((ret = lireLigne(sd, buf)) == 0);
	if (ret == -1) {
		erreur_IO("sendServ - lireLigne");
	}
	else if (ret == LIGNE_MAX) {
		erreur("sendServ - line too long\n");
	}
	if (strcmp(buf, "OK") == 0) {
		printf("\treceived\n");
	}
	else {
		erreur("sendServ - bad answer from server: %s\n", buf);
	}
}

