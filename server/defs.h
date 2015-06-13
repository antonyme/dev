#ifndef DEFS_H
#define DEFS_H

#define    CMD            "serveur"
#define    NTHREADS       5
#define    MILLISECONDES  1000
#define    ATTENTE        2000*MILLISECONDES
#define    TMAX           50

typedef struct objet {
	int id;  //identifiant de l'objet
	int prix_ini;  //prix initial de l'objet
	int prix_cur;  //prix actuel de l'objet. Ce prix change à chaque nouvelle enchère
	char type;  // bijou voiture maison (etc) peut être remplacé par un char*
	int rare;  // rareté du bien (de 1 à 10: l'objet de rareté 10 est le plus rare)
} OBJET;


typedef struct acheteur {
	char nom[TMAX]; // nom de l'acheteur
	char personnalite[TMAX]; //personnalité de l'acheteur: oisif, collectionneur, lambda, milliardaire, avare
	int argent_ini;  // argent initial lorsque s'ouvre la vente avec le premier objet.
	int argent_cur;  // argent à l'instant t
	int possession_B;  //  possession de bijoux à l'instant t
	int possession_V;  // possession de voitures à l'instant t
	int possession_M;  // possession de maisons à l'instant t
	int latence;  // temps de latence de l'acheteur. 0 si non intéressé.
	int prix_prop;  // prix proposé par l'acheteur
} ACHETEUR;

#endif