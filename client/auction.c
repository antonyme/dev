/*
 * Module de gestion des actions du client dans la vente aux enchères
 *//*
#include "pse.h"
#include "defs.h"
#include "auction.h"
 
 //pour UN client, la fonction fglobale va l'affilier à une fonction en fonction de son caractère
int fglobale (ACHETEUR *monsieurx, OBJET *bidule)
{
	if (strcmp(monsieurx->personnalite, "oisif"))
	{foisif(monsieurx, bidule);}
	if (strcmp(monsieurx->personnalite, "collectionneur"))
	{fcollectionneur(monsieurx, bidule);}
	if (monsieurx->personnalite=='milliardaire')
	{milliardaire(monsieurx, bidule);}
	if (monsieurx->personnalite=='avare')
	{avare(monsieurx, bidule);}
	else                                           // si l'utilisateur a entré lambda ou autrechose que oisif, collectionneur, milliardaire, avare.
	{flambda(monsieurx, bidule);}
}


void initacheteur (ACHETEUR *monsieurx)
{
	monsieurx->argent_cur = monsieurx->argent_ini;
}

//*************fonctions qui permettent de déterminer le temps de latence*********************************
int flambda (ACHETEUR *monsieurx, OBJET *bidule)
{
	if ((bidule->type=='B'&& monsieurx->possession_B<=3 || bidule->type=='V'&& monsieurx->possession_V<=3 || bidule->type=='M'&& monsieurx->possession_M<=3) && monsieurx->argent_cur<=(bidule->prix_cur)/2)
	{
		monsieurx->latence = 5;
		monsieurx->prix_prop = bidule->prix_cur + 0,1*(bidule->prix_cur);
	}
	else monsieurx->latence = 0;
	}
}


int foisif (ACHETEUR*monsieurx, OBJET*bidule)
{
	if ((bidule->type=='B'&& monsieurx->possession_B<=3 || bidule->type=='V'&& monsieurx->possession_V<=3 || bidule->type=='M'&& monsieurx->possession_M<=3) && monsieurx->argent_cur<=(bidule->prix_cur)/2)
	{
		monsieurx->latence = 8;
		monsieurx->prix_prop = bidule->prix_cur + 0,2*(bidule->prix_cur);
	}
	else monsieurx->latence = 0;
	}
}

int fmilliardaire (ACHETEUR*monsieurx, OBJET *bidule)
{
	if(bidule->rare>=6 && monsieurx->argent_cur >= 1,5*OBJET->prix_cur)
	{
		bidule->prix_cur = bidule->prix_cur + 0,5*bidule->prix_cur;
		monsieurx->latence = 2;
	}
	else monsieurx->latence = 0;
	}
}

int fcollectionneur (ACHETEUR*monsieurx, OBJET *bidule)
{
	if (bidule->rare >=8 && monsieurx->argent_cur >= 1,1*OBJET->prix_cur)
	{
		bidule->prix_cur = bidule->prix_cur + 0,05*bidule->prix_cur;
		monsieurx->latence = 1;
	}
	else monsieurx->latence = 0;
	}
}


int favare (ACHETEUR*monsieurx, OBJET *bidule)
{
	if (bidule->rare >= 5 && monsieurx->argent_cur >= 4*OBJET->prix_cur)
	{
		bidule->prix_cur = bidule->prix_cur + 0,01*bidule->prix_cur;
		monsieurx->latence = 6;
	}
	else monsieurx->latence = 0;
	}
}

void carte_identite () // pas fini, je finirai plus tard
{
	int i;
	for (i=1; i<=NBA; i++)
	{
		printf(entrez le nom, )
	}
}
*/