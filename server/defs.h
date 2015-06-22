#ifndef DEFS_H
#define DEFS_H

#define    CMD            "serveur"
#define    NTHREADS       5
#define	   START_WAIT     15
#define    AUCT_WAIT	  20

#include "../struct.h"

DataSpec cohorte[NTHREADS];

int clientMessage, endObj, end;

int nbClients;
int clients[NTHREADS];
int nbObjs;
OBJET objs[TMAX], *curObj;

float bid;
int bidder;
pthread_cond_t condBid;
pthread_mutex_t mutexBid;

sem_t semWork;

pthread_barrier_t auctionStart;

#endif
