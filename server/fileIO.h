#ifndef FILEIO_H
#define FILEIO_H

/*
 * Remplit la structure décrivant l'objet (obj) à partir du ficher de
 * sauvegarde (name) dans data/objects
 */
int fillInfos (OBJET *obj, char *name);

/*
 * Demande à l'utilisateur la liste d'objets souhaité pour la vente et réalise
 * les appels de fillInfos
 */
void fillAllObjs ();

#endif