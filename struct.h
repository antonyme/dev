#ifndef STRUCT_H
#define STRUCT_H

#define    TMAX           50
#define	   LATMAX         100000

typedef struct acheteur {
	char nom[TMAX]; // nom de l'acheteur
	char personnalite[TMAX]; //personnalité de l'acheteur: oisif, collectionneur, lambda, milliardaire, avare
	float argent_ini;  // argent initial lorsque s'ouvre la vente avec le premier objet.
	float argent_cur;  // argent à l'instant t
	int possession_B;  //  possession de bijoux à l'instant t
	int possession_V;  // possession de voitures à l'instant t
	int possession_M;  // possession de maisons à l'instant t
	float latence;  // temps de latence de l'acheteur. LATMAX si non intéressé.
	float prix_prop;  // prix proposé par l'acheteur
} ACHETEUR;

typedef struct objet {
	char nom[TMAX];  //identifiant de l'objet.
	float prix_ini;  //prix initial de l'objet.
	float prix_cur;  //prix actuel de l'objet. Ce prix change à chaque nouvelle enchère
	char type;  // bijou voiture maison (etc) peut être remplacé par un char*
	int rare;  // rareté du bien (de 1 à 10: l'objet de rareté 10 est le plus rare)
} OBJET;

#endif