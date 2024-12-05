#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include "monitor_signal_continue.h"

#define DIM 3
#define VAR_CONDITIONS 3

#define PRODUTTORE_VELOCE 1
#define PRODUTTORE_LENTO 1
#define MESSAGGIO_DISPONIBILE 3

typedef struct{
	int buffer[DIM];

	int testa;
	int coda;
	int count;
	int num_lettori_alta_priorita;
	Monitor m;
}PriorityProdCons;


void inizializza_prod_cons(PriorityProdCons* p);
void produci_alta_prio(PriorityProdCons* p );
void produci_bassa_prio(PriorityProdCons* p);
void consuma(PriorityProdCons* p);
void rimuovi_prod_cons(PriorityProdCons* p);

#endif
