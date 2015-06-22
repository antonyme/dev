#include "pse.h"
#include "defs.h"
#include "save.h"

void newClient (void) {
	ACHETEUR new;
	FILE *fp;
	char fileName[TMAX] = "./../data/clients/";

	printf("Nom?\n> ");
	scanf("%s", new.nom);
	printf("entrez la personnalite. oisif, collectionneur, milliardaire, avare, lambda\n>");
	scanf("%s", new.personnalite);
	printf("entrez le capital initial\n> ");
	scanf("%f", &new.argent_ini);
	printf("entrez le nombre de bijoux\n> ");
	scanf("%d", &new.possession_B);
	printf("entrez le nombre de voitures\n> ");
	scanf("%d", &new.possession_V);
	printf("entrez le nombre de maisons\n> ");
	scanf("%d", &new.possession_M);
	printf("\n Recapitulatif des informations renseignees pour Monsieur %s: \n Personnalite: %s \n Capital initial : %.2f \n Bijoux : %d \n Voitures : %d \n Maisons : %d \n \n", new.nom,new.personnalite,new.argent_ini,new.possession_B,new.possession_V,new.possession_M);

	strcat(fileName, new.nom);  // on appelle le fichier par le prénom du mec
	fp = fopen(fileName, "w+");
	if (fp == NULL) {
		erreur_IO("open");
	}
	fwrite(&new, sizeof(ACHETEUR), 1, fp);
	fclose(fp);
}

void newObjet (void) {
	OBJET newo;
	FILE *fo;
	char filename[TMAX] = "./../data/objects/";

	printf("Nom?\n> ");
	scanf("%s", newo.nom);
	printf("entrez le prix initial\n> ");
	scanf("%f",&newo.prix_ini );
	printf("entrez le type\n> ");
	scanf(" %c",&newo.type);
	printf("entrez la rarete, valeur allant de 1 à 10\n> ");
	scanf("%d", &newo.rare);
	printf("\n Recapitulatif des informations renseignees pour l'objet %s \n Prix initial : %.2f \n Type : %c \n Rarete : %d \n \n", newo.nom, newo.prix_ini, newo.type, newo.rare);

	strcat(filename, newo.nom);  // on appelle le fichier par le nom de l'objet

	fo = fopen(filename, "w+");
	if (fo == NULL) {
		erreur_IO("open");
	}
	fwrite(&newo, sizeof(OBJET), 1, fo);
	fclose(fo);
}