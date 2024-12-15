#include <stdio.h>
#include <stdlib.h>

#include "prodcons.h"

#define NUM_THREAD_PROD 3
#define NUM_THREAD_CONS 3
#define NUM_PRODUZIONI 10
#define NUM_CONSUMAZIONI 10
#define CONTEGGIO 5

void * thread_produttore(void *);
void * thread_consumatore(void *);
void * thread_conteggio(void *);


int main() {

    /* TBD: Creare un oggetto monitor di tipo "MonitorProdCons" */
    MonitorProdCons *m=malloc(sizeof(MonitorProdCons));
    pthread_t threadsProduttore[NUM_THREAD_PROD];
    pthread_t threadsConsumatore[NUM_THREAD_CONS];
    pthread_t threadConteggio;
    init_monitor(m);

    for(int i=0; i<NUM_THREAD_PROD; i++) {
        pthread_create(&threadsProduttore[i],NULL,thread_produttore,m);
        /* TBD: Creare un thread produttore, fargli eseguire la funzione "thread_produttore" */
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {
        pthread_create(&threadsConsumatore[i],NULL,thread_consumatore,m);
        /* TBD: Creare un thread consumatore, fargli eseguire la funzione "thread_consumatore" */
    }

    /* TBD: Creare un thread di "conteggio", fargli eseguire la funzione "thread_conteggio" */
    pthread_create(&threadConteggio,NULL,thread_conteggio,m);
    /* TBD: Attendere la terminazione dei thread */
    for (int i = 0; i < NUM_THREAD_PROD; i++)
    {
        pthread_join(threadsProduttore[i],NULL);
    }
    for (int i = 0; i < NUM_THREAD_CONS; i++)
    {
        pthread_join(threadsConsumatore[i],NULL);
    }
    pthread_join(threadConteggio,NULL);
    
    remove_monitor(m);
    /* TBD: Deallocare l'oggetto monitor */
    free(m);
}

void * thread_produttore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *m=(MonitorProdCons*) x;
    for(int i=0; i<NUM_PRODUZIONI; i++) {

        /* TBD: Completare la chiamata a produzione() */

        int val = rand() % 10;
        produzione(m, val);

        printf("[PRODUTTORE] Ho prodotto: %d\n", val);

    }
    return NULL;
}

void * thread_consumatore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *m=(MonitorProdCons*) x;
    for(int i=0; i<NUM_CONSUMAZIONI; i++) {

        /* TBD: Completare la chiamata a consumazione() */

        int val = consumazione(m);

        printf("[CONSUMATORE] Ho consumato: %d\n", val);
    }
    
    return NULL;
}


void * thread_conteggio(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *m=(MonitorProdCons*) x;

    int consumazioni_effettuate = 0;

    int consumazioni_da_effettuare = NUM_CONSUMAZIONI * NUM_THREAD_CONS;

    while(consumazioni_da_effettuare > 0) {

        int conteggio;

        if( consumazioni_da_effettuare >= CONTEGGIO ) {

            // Vi sono 5 o più consumazioni ancora da effettuare,
            // si pone a 5 il conteggio da attendere
            conteggio = CONTEGGIO;
        }
        else {

            // Vi sono meno di 5 consumazioni ancora da effettuare,
            // si pone il conteggio da attendere a "consumazioni_da_effettuare"
            conteggio = consumazioni_da_effettuare;
        }

        /* TBD: Completare la chiamata ad "attendi_consumazioni()" */

        int n = attendi_consumazioni(m, conteggio);

        consumazioni_effettuate += n;

        consumazioni_da_effettuare -= n;

        printf("[CONTEGGIO] Sono stati consumati in totale %d elementi\n", consumazioni_effettuate);
    }

    return NULL;
}
