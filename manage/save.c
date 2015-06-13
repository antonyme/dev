#include "pse.h"
#include "../client/defs.h"
#include "save.h"

void newClient (void) {
	ACHETEUR new;
	FILE *fp;
	char fileName[TMAX] = "./../data/clients/";
	
	printf("Nom?\n> ");
	scanf("%s", new.nom);
	//TODO demander à l'utilisateur de remplir les champs de new
	
	strcat(fileName, new.nom);
	fp = fopen(fileName, "w+");
	if (fp == NULL) {
		erreur_IO("open");
	}
	fwrite(&new, sizeof(ACHETEUR), 1, fp);
	fclose(fp);
}