#ifndef DEFS_H
#define DEFS_H

#define    CMD            "serveur"
#define    NTHREADS       5
#define    MILLISECONDES  1000
#define    ATTENTE        2000*MILLISECONDES

#include "../struct.h"

DataSpec cohorte[NTHREADS];

int state;

int nbObjs;
int objInSale;
OBJET objs[TMAX];
pthread_mutex_t mutexObjs;

float bid;
pthread_cond_t condBid;
pthread_mutex_t mutexBid;

sem_t sem_work;

#endif
