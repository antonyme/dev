/* 
 * Module de gestion de la connection au client et de la
 * communication avec celui-ci.
 */
#include <stdarg.h>
#include "pse.h"
#include "defs.h"
#include "clientIO.h"

int createEcoute (char strPort[]) {
	int ecoute, ret;
	struct sockaddr_in adrEcoute;
	short port;
	
	port = (short) atoi(strPort);
	
	printf("%s: creating a socket\n", CMD);
	ecoute = socket (AF_INET, SOCK_STREAM, 0);
	if (ecoute < 0) {
		erreur_IO("socket");
	}
	
	adrEcoute.sin_family = AF_INET;
	adrEcoute.sin_addr.s_addr = INADDR_ANY;
	adrEcoute.sin_port = htons(port);
	printf("%s: binding to INADDR_ANY address on port %d\n", CMD, port);
	ret = bind (ecoute,  (struct sockaddr *) &adrEcoute, sizeof(adrEcoute));
	if (ret < 0) {
		erreur_IO("bind");
	}
	
	printf("%s: listening to socket\n", CMD);
	ret = listen (ecoute, 5);
	if (ret < 0) {
		erreur_IO("listen");
	}
	
	return ecoute;
}

void sendCli (int sd, const char* format, ...) {
	int ret;
	char buf[LIGNE_MAX];
	va_list args;
	
	va_start(args, format);
	vsnprintf(buf, LIGNE_MAX, format, args);
	va_end (args);
	ret = ecrireLigne(sd, buf);
	if (ret == -1) {
		erreur_IO("ecrireLigne");
	}
}

void recvCli (int sd, char* buf) {
	int ret;
	
	while ((ret = lireLigne(sd, buf)) == 0);
	if (ret == -1) {
		erreur_IO("lireLigne");
	}
	else if (ret == LIGNE_MAX) {
		erreur("line too long");
	}
}
