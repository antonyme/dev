/* 
 * Module de gestion de la connection au serveur et de la
 * communication avec celui-ci.
 */
#include <stdarg.h>
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

void sendServ (int sd, const char* format, ...) {
	int ret;
	char buf[LIGNE_MAX];
	va_list args;
	
	va_start(args, format);
	vsnprintf(buf, LIGNE_MAX, format, args);
	va_end (args);
	printf("IO: send: %s\n", buf);
	ret = ecrireLigne(sd, buf);
	if (ret == -1) {
		erreur_IO("ecrireLigne");
	}

	while ((ret = lireLigne(sd, buf)) == 0);
	if (ret == -1) {
		erreur_IO("lireLigne");
	}
	else if (ret == LIGNE_MAX) {
		erreur("line too long\n");
	}
	if (strcmp(buf, "OK") != 0) {
		erreur("bad answer from server: %s\n", buf);
	}
}

void recvServ (int sd, char* buf) {
	int ret;
	char bufRep[LIGNE_MAX] = "OK";
	
	while ((ret = lireLigne(sd, buf)) == 0);
	printf("IO: recv: %s\n", buf);
	if (ret == -1) {
		erreur_IO("lireLigne");
	}
	else if (ret == LIGNE_MAX) {
		erreur("line too long");
	}
	ret = ecrireLigne(sd, bufRep);
	if (ret == -1) {
		erreur_IO("ecrireLigne");
	}
}
