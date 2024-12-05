#ifndef HEADERM_H
#define HEADERM_H

#include "header.h"

#define SCORTA_INIZIALE 20
#define IN_USO 0
#define LIBERO 0

#define SYNCH 0
#define MUTEX_L 1

typedef struct {
    // TODO aggiungere le variabili necessarie alla sincronizzazione
    int num_lettori[MAX_PRODOTTI];
    int scorta[MAX_PRODOTTI];

    int ds_sem_mutexl;
    int ds_sem_sync;
}Magazzino;


void init_magazzino(Magazzino *magazzino);
void destroy_magazzino(Magazzino *magazzino);

void lettore(Magazzino *magazzino);
void scrittore(Magazzino *magazzino);

void InizioLettura(Magazzino *magazzino, int id_prodotto);
void FineLettura(Magazzino *magazzino, int id_prodotto);
void InizioScrittura(Magazzino *magazzino, int id_prodotto);
void FineScrittura(Magazzino *magazzino, int id_prodotto);



#endif
