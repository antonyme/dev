/*
 * Module de gestion des actions du client dans la vente aux enchères
 */
#include "pse.h"
#include "defs.h"
#include "auction.h"

 //pour UN client, la fonction fglobale va l'affilier à une fonction en fonction de son caractère
void getHangTime (ACHETEUR *monsieurx, OBJET *bidule)
{
	
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


float rand_a_b(float min, float max){
	return ( (float)rand()/RAND_MAX ) * (max - min) + min;
}


void foisif (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ( ((bidule->type=='B'&& monsieurx->possession_B<=3) || 
			(bidule->type=='V'&& monsieurx->possession_V<=3) || 
			(bidule->type=='M'&& monsieurx->possession_M<=3) ) && 
			monsieurx->argent_cur >= 2*bidule->prix_cur && 
			bidule->prix_cur <= 2*(bidule->prix_ini) )
	{
		monsieurx->latence = rand_a_b(2, 25);
		monsieurx->prix_prop = bidule->prix_cur + rand_a_b(0.1,0.5)*(bidule->prix_cur);
	}
	else monsieurx->latence = LATMAX;
}

void fcollectionneur (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ((bidule->rare >=8) && 
		(monsieurx->argent_cur >= 1.1*bidule->prix_cur) && 
		bidule->prix_cur <= 4*(bidule->prix_ini))
	{
		monsieurx->prix_prop = bidule->prix_cur + rand_a_b(0.05,0.15)*(bidule->prix_cur);
		monsieurx->latence = rand_a_b(1,5);
	}
	else {monsieurx->latence = LATMAX;}
}


void fmilliardaire (ACHETEUR *monsieurx, OBJET *bidule)
{
	if((bidule->rare >= 6) && 
		(monsieurx->argent_cur >= 3*bidule->prix_cur) && 
		bidule->prix_cur <= 10*bidule->prix_ini)
	{
		monsieurx->prix_prop = bidule->prix_cur + rand_a_b(0.5,0.7)*(bidule->prix_cur);
		monsieurx->latence = rand_a_b(3,8);
	}
	else {monsieurx->latence = LATMAX;}
}


void favare (ACHETEUR *monsieurx, OBJET *bidule)
{
	if (bidule->rare >= 5 && monsieurx->argent_cur >= 4*bidule->prix_cur)
	{
		monsieurx->prix_prop = bidule->prix_cur + rand_a_b(0.02,0.04)*bidule->prix_cur;
		monsieurx->latence = rand_a_b(5,20);
	}
	else {monsieurx->latence = LATMAX;}
}


void flambda (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ( ((bidule->type=='B'&& monsieurx->possession_B<=3) || 
		(bidule->type=='V'&& monsieurx->possession_V<=3) || 
		(bidule->type=='M'&& monsieurx->possession_M<=3)) && 
		monsieurx->argent_cur >= (bidule->prix_cur)*2 && 
		bidule->prix_cur <= 3*(bidule->prix_ini))
	{
		monsieurx->latence = /*rand_a_b(3,10)*/ 7.24;
		monsieurx->prix_prop = bidule->prix_cur + rand_a_b(0.2,0.4)*(bidule->prix_cur);
	}
	else {monsieurx->latence = LATMAX;}
}


void fjoueur (ACHETEUR *monsieurx, OBJET *bidule)
{
	char rep;
	printf("Voulez-vous proposer un nouveau prix?\n Repondre par o ou n: \n> ");
	scanf(" %c", &rep);
	if (rep =='o')
	{
		printf("Quel nouveau prix proposez-vous?\n");
		scanf("%f", &monsieurx->prix_prop);
		monsieurx->latence = 0;
	}
	else
	{
		monsieurx->latence = LATMAX;
	}
}