#include "pse.h"
#include "defs.h"
#include "main.h"
#include "save.h"

int main (int argc, char *argv[]) {

	if (argc != 2) {
		erreur("usage: %s action\nlist actions:\n\tnew_client\n\tnew_object\n", argv[0]);
	}
	if (strcmp("new_client", argv[1]) == 0) {
		newClient();
	}
	else if (strcmp("new_object", argv[1]) == 0) {
		newObjet();
	}
	else printf("Mauvaise commande\n");

	return 0;
}