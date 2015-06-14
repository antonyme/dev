#ifndef AUCTION_H
#define AUCTION_H

void testHang (ACHETEUR *monsieurx, OBJET *bidule);

void getHangTime (ACHETEUR *monsieurx, OBJET *bidule);

void flambda (ACHETEUR *monsieurx, OBJET *bidule);
void foisif (ACHETEUR *monsieurx, OBJET*bidule);
void fmilliardaire (ACHETEUR *monsieurx, OBJET *bidule);
void fcollectionneur (ACHETEUR *monsieurx, OBJET *bidule);
void favare (ACHETEUR *monsieurx, OBJET *bidule);
void fjoueur (ACHETEUR *monsieurx, OBJET *bidule);

void traitementjoueur (ACHETEUR *monsieurx, OBJET *bidule);

double alea(double inf, double sup);

int rand_a_b(int min, int max);




#endif