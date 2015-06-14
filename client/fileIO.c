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
		erreur_IO("fopen");
	}
	fread(me, sizeof(ACHETEUR), 1, fp);
}