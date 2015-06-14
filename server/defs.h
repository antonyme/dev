#ifndef DEFS_H
#define DEFS_H

#define    CMD            "serveur"
#define    NTHREADS       5
#define    MILLISECONDES  1000
#define    ATTENTE        2000*MILLISECONDES

#include "../struct.h"

DataSpec cohorte[NTHREADS];
sem_t sem_work;

#endif
