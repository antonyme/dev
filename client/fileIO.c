/*
 * Module de sauvegarde ou de chargement de données dans et depuis
 * des fichers
 */
#include "pse.h"
#include "defs.h"
#include "fileIO.h"

void fillInfos (ACHETEUR *me, char *name) {
	FILE *fp;
	char fileName[TMAX] = "./../data/clients/";
	
	strcat(fileName, name);
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		if (errno != ENOENT) {
			erreur_IO("fopen");
		}
		erreur("le profil client de %s n'existe pas\n");
	}
	if(fread(me, sizeof(ACHETEUR), 1, fp) == 0) {
		erreur_IO("fread");
	}
}

void showInfos (ACHETEUR *me) {
	printf("\nMonsieur %s:\n Personnalite: %s\n Capital initial: %.2f\n Bijoux: %d\n Voitures: %d\n Maisons: %d\n \n", me->nom,me->personnalite,me->argent_ini,me->possession_B,me->possession_V,me->possession_M);
}