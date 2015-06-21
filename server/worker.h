#ifndef WORKER_H
#define WORKER_H

/*
 * Fonction appellée lors de la création d'un worker.
 * Attend qu'un client lui soit attribué puis le gère.
 */
void *traiterRequete (void *arg);

/*
 * Crée un pool de NTHREADS workers
 */
void createCohorte ();

#endif