#include "pse.h"
#include "../client/defs.h"
#include "main.h"
#include "save.h"

int main (int argc, char *argv[]) {
	
	if (argc != 2) {
		erreur("usage: %s action\n", argv[0]);
	}
	if (strcmp("new_client", argv[1]) == 0) {
		newClient();
	}
	else if (strcmp("new_object", argv[1]) == 0) {
		//TODO newObject();
	}
	else printf("Mauvaise commande\n");
	
	return 0;
}