/*
 * Module de gestion des actions du client dans la vente aux enchères
 */
#include "pse.h"
#include "defs.h"
#include "auction.h"


void testHang (ACHETEUR *monsieurx, OBJET *bidule) {
	monsieurx->latence = rand_a_b(5, 15);
	monsieurx->prix_prop = bidule->prix_cur + rand_a_b(100, 1000);
}

 //pour UN client, la fonction fglobale va l'affilier à une fonction en fonction de son caractère
void getHangTime (ACHETEUR *monsieurx, OBJET *bidule)
{
	printf("%s\n", monsieurx->personnalite);
	if (strcmp((monsieurx->personnalite), "oisif")==0)  // si je compare monsieurx->personnalite c'est une adresse.
		foisif(monsieurx, bidule);

	else if (strcmp(monsieurx->personnalite, "collectionneur")==0)
		fcollectionneur(monsieurx, bidule);

	else if (strcmp(monsieurx->personnalite,"milliardaire")==0)
		fmilliardaire(monsieurx, bidule);

	else if (strcmp(monsieurx->personnalite,"avare")==0)
		favare(monsieurx, bidule);

	else if (strcmp(monsieurx->personnalite,"joueur")==0)
		fjoueur(monsieurx, bidule);
		
	else                                           // si l'utilisateur a entré lambda ou autrechose que oisif, collectionneur, milliardaire, avare.
		flambda(monsieurx, bidule);

}


//************* fonctions qui permettent de déterminer le temps de latence *********************************

double alea(double inf, double sup){
	 return ( rand()/(double)RAND_MAX ) * (sup-inf) + inf;
}


int rand_a_b(int min, int max){
    return rand() % (max - min) + min;
}


void foisif (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ( ((bidule->type=='B'&& monsieurx->possession_B<=3) || (bidule->type=='V'&& monsieurx->possession_V<=3) || (bidule->type=='M'&& monsieurx->possession_M<=3) ) && monsieurx->argent_cur >= (bidule->prix_cur)*2 && bidule->prix_cur <=2*(bidule->prix_ini) )
	{
		monsieurx->latence = rand_a_b(6, 10);
		monsieurx->prix_prop = bidule->prix_cur + 0.4*(bidule->prix_cur);
	}
	else {monsieurx->latence = LATMAX;}
}

void fcollectionneur (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ((bidule->rare >=5) 
		&& (monsieurx->argent_cur >= 1.1*bidule->prix_cur) 
		&& bidule->prix_cur <= 4*(bidule->prix_ini))
	{
		monsieurx->prix_prop = bidule->prix_cur + 0.10*(bidule->prix_cur);
		monsieurx->latence = rand_a_b(1,2);
	}
	else {monsieurx->latence = LATMAX;}
}


void fmilliardaire (ACHETEUR *monsieurx, OBJET *bidule)
{
	if((bidule->rare >= 3) && (monsieurx->argent_cur >= 3*bidule->prix_cur) && bidule->prix_cur <= bidule->prix_ini)
	{
		monsieurx->prix_prop = bidule->prix_cur + 1*(bidule->prix_cur);
		monsieurx->latence = rand_a_b(2,4);
	}
	else {monsieurx->latence = LATMAX;}
}


void favare (ACHETEUR *monsieurx, OBJET *bidule)
{
	if (bidule->rare >= 2 && monsieurx->argent_cur >= 4*bidule->prix_cur)
	{
		monsieurx->prix_prop = bidule->prix_cur + 0.04*bidule->prix_cur;
		monsieurx->latence = rand_a_b(5,8);
	}
	else {monsieurx->latence = LATMAX;}
}


void flambda (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ( ((bidule->type=='B'&& monsieurx->possession_B<=3) || (bidule->type=='V'&& monsieurx->possession_V<=3) || (bidule->type=='M'&& monsieurx->possession_M<=3)) && monsieurx->argent_cur >= (bidule->prix_cur)*2 && bidule->prix_cur <= 3*(bidule->prix_ini))
	{
		monsieurx->latence = rand_a_b(3,7);
		monsieurx->prix_prop = bidule->prix_cur + 0.5*(bidule->prix_cur);
	}
	else {monsieurx->latence = LATMAX;}
}


void fjoueur (ACHETEUR *monsieurx, OBJET *bidule)
{
	char rep;
	printf("le prix actuel de l'objet est %f\n Voulez-vous proposer un nouveau prix?\n Repondre par o ou n: \n", bidule->prix_cur);
	scanf(" %c", &rep);
	if (rep =='o')
	{
		printf("Quel nouveau prix proposez-vous?\n");
		scanf("%f", &monsieurx->prix_prop);
		if (monsieurx->prix_prop > bidule->prix_cur)
		{
		printf("le prix propose par Monsieur %s est %f\n", monsieurx->nom, monsieurx->prix_prop);
		traitementjoueur(monsieurx, bidule);
		}
		else if (monsieurx->prix_prop <= bidule->prix_cur)
			{
			printf("le prix que vous proposez est inferieur ou egal au prix actuel.\n");
			monsieurx->latence = LATMAX;
			}
	}
	else
	{
		monsieurx->latence = LATMAX;
		printf("l'objet %s n'interesse pas Monsieur %s \n", bidule->nom, monsieurx->nom);
	}
}


void traitementjoueur (ACHETEUR *monsieurx, OBJET *bidule)
{
	if (monsieurx->prix_prop <= 3*(bidule->prix_cur))
		monsieurx->latence = 10*((monsieurx->prix_prop - bidule->prix_cur)/(bidule->prix_cur));
	else monsieurx->latence = rand_a_b (1,10);
}