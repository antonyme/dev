#ifndef FILEIO_H
#define FILEIO_H

/*
 * Remplit la structure décrivant le client (me) à partir du ficher de
 * sauvegarde (name) dans data/clients
 */
void fillInfos (ACHETEUR *me, char *name);

/*
 * Affiche le contenu de la structure
 */
void showInfos (ACHETEUR *me);

#endif