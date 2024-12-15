#ifndef _PRODCONS_
#define _PRODCONS_

#include <pthread.h>

#define DIM 5

typedef struct {

    int buffer[DIM];
    int testa;
    int coda;
    int conteggio_elementi;        /* numero di buffer occupati */
    int conteggio_consumazioni;    /* conteggio consumazioni effettuate */

    pthread_mutex_t mutex;
    pthread_cond_t cv_prod;
    pthread_cond_t cv_cons;
    pthread_cond_t cv_conteggio;

} MonitorProdCons;


void init_monitor(MonitorProdCons * p);
void produzione(MonitorProdCons * p, int val);
int consumazione(MonitorProdCons * p);
int attendi_consumazioni(MonitorProdCons * p, int soglia);
void remove_monitor(MonitorProdCons * p);

#endif