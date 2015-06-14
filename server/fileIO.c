/*
 * Module de sauvegarde ou de chargement de données dans et depuis
 * des fichers
 */
#include "pse.h"
#include "defs.h"
#include "fileIO.h"

int fillInfos (OBJET *obj, char *name) {
	FILE *fp;
	char fileName[TMAX] = "./../data/objects/";
	
	strcat(fileName, name);
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		if (errno != ENOENT) {
			erreur_IO("fopen");
		}
		return -1;
	}
	if (fread(obj, sizeof(OBJET), 1, fp) == 0) {
		erreur_IO("fread");
	}
	if (fclose(fp) != 0) {
		erreur_IO("fclose");
	}
	return 0;
	
}

void fillAllObjs () {
	int i = 0;
	char name[TMAX], rep;
	
	printf("Saisie des objets à mettre en vente (max: %d):\n", TMAX);
	while (i < TMAX) {
		printf("Objet %d:\n> ", i);
		scanf("%s", name);
		if (fillInfos(&objs[i], name) == -1) {
			printf("Objet non existant\n");
		}
		else {
			i++;
		}
		printf("Continue (o/n)?\n");
		scanf(" %c", &rep);
		if (rep != 'o') break;
	}
	if (i == TMAX) {
		printf("Nombre maximal d'objets en vente atteint\n");
	}
	nbObjs = i;
}