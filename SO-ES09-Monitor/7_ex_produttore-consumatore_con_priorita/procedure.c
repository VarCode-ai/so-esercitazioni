/*IMPLEMENTAZIONE DELLE PROCEDURE*/

#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* TBD: Definire delle macro per identificare le variabili condition */


void inizializza_prod_cons(PriorityProdCons* p){

	
	/* TBD: Inizializzare le variabili della struttura (testa, coda, ...) */
	p->testa=0;
	p->coda=0;
	p->count=0;
	p->num_lettori_alta_priorita=0;
	init_monitor(&(p->m),VAR_CONDITIONS);

	/* TBD: Inizializzare il sotto-oggetto "Monitor", chiamando init_monitor(...).
	        Scegliere accuratamente il numero di variabili condition */

}

void rimuovi_prod_cons(PriorityProdCons* p){
	remove_monitor(&(p->m));
	/* TBD: Deallocare il sotto-oggetto "Monitor", chiamando remove_Monitor(...). */
}


void produci_alta_prio(PriorityProdCons* p){

	int value;

	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(p->m));
	//Aumenta il numero di produttori
	printf("Produttore 1 entrato nel monitor...\n");

	p->num_lettori_alta_priorita++;

    // Attendere se il buffer è pieno
    if (p->count == DIM) {
        wait_condition(&(p->m), PRODUTTORE_VELOCE);
    }

    // Decrementare il numero di produttori veloci in attesa
    p->num_lettori_alta_priorita--;
	
	//Produco il valore
	value = rand() % 12 ;

	//Modifico il buffer
	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	//Il buffer non è vuoto
	printf("Produttore 1 con pid %d ha prodotto %d\n",getpid(),value);	
	
	/* TBD: Effettuare la signal_condition per i consumatori in attesa */
	signal_condition(&(p->m),MESSAGGIO_DISPONIBILE);
	/* TBD: Uscire dal monitor */
	leave_monitor(&(p->m));

}

void produci_bassa_prio(PriorityProdCons* p){

	int value;
	
	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(p->m));
	//Aumenta il numero di produttori
	printf("Produttore 2 entrato nel monitor...\n");
	
	/* TBD: Sospendere qui il processo se il vettore di buffer è pieno */
	 if (p->count == DIM || queue_condition(&(p->m), PRODUTTORE_VELOCE) > 0) {
        printf("Produttore lento con pid %d in attesa...\n", getpid());
        wait_condition(&(p->m), PRODUTTORE_LENTO);
    }
	
	
	//Produco il valore
	value = 13 + (rand() % 12) ;

	//Modifico il buffer
	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	//Il buffer non è vuoto
	printf("Produttore 2 con pid %d ha prodotto %d\n",getpid(),value);	

	/* TBD: Effettuare la signal_condition per i consumatori in attesa */
	signal_condition(&(p->m),MESSAGGIO_DISPONIBILE);
	leave_monitor(&(p->m));
	/* TBD: Uscire dal monitor */

}

void consuma(PriorityProdCons* p){

	int value;

	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(p->m));
	/* TBD: Sospendere qui il processo se il vettore di buffer è vuoto */
	if (p->count==0)
	{
		wait_condition(&(p->m),MESSAGGIO_DISPONIBILE);
	}
	

	value = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	p->count--;

	printf("Consumatore con pid %d ha consumato valore %d\n",getpid(),value);

	printf("numero produttori alta priorità %d\n",p->num_lettori_alta_priorita);
	/*if (p->num_lettori_alta_priorita>0)
	{
		signal_condition(&(p->m),PRODUTTORE_VELOCE);
		printf("signal veloce");

	}
	else{
		printf("signal lento");
		signal_condition(&(p->m),PRODUTTORE_LENTO);
	}*/
	if (queue_condition(&(p->m), PRODUTTORE_VELOCE) > 0) {
        signal_condition(&(p->m), PRODUTTORE_VELOCE);
    } else if (queue_condition(&(p->m), PRODUTTORE_LENTO) > 0) {
        signal_condition(&(p->m), PRODUTTORE_LENTO);
    }
	leave_monitor(&(p->m));
	
	/* TBD: Effettuare la signal_condition per attivare un produttore.
	 *      Si attivi un produttore di priorità alta se presente, altrimenti,
	 *      si attivi un produttore di priorità bassa.
	 * 
	 * 		Per determinare se ci sono produttori ad alta priorità in attesa,
	 * 		si introduca una variabile "num_lettori_alta_priorita", che i produttori
	 *      incrementano subito prima di fare wait_condition(), e che decrementano
	 * 		subito dopo aver eseguito wait_condition().
	 * 
	 * 		In alternativa, utilizzare la funzione "queue_condition()".
	 */


	/* TBD: Uscire dal monitor */
}
