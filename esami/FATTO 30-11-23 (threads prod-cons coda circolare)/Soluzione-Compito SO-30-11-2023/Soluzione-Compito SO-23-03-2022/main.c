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

    pthread_t produttori[NUM_THREAD_PROD];
    pthread_t consumatori[NUM_THREAD_CONS];
    pthread_t conteggio;

    MonitorProdCons * p = malloc(sizeof(MonitorProdCons));

    init_monitor(p);

    for(int i=0; i<NUM_THREAD_PROD; i++) {

        pthread_create(&produttori[i], NULL, thread_produttore, p);
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {

        pthread_create(&consumatori[i], NULL, thread_consumatore, p);
    }


    pthread_create(&conteggio, NULL, thread_conteggio, p);


    for(int i=0; i<NUM_THREAD_PROD; i++) {

        pthread_join(produttori[i], NULL);
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {

        pthread_join(consumatori[i], NULL);
    }

    pthread_join(conteggio, NULL);

    remove_monitor(p);

    free(p);
}

void * thread_produttore(void * x) {

    MonitorProdCons * p = x;

    for(int i=0; i<NUM_PRODUZIONI; i++) {

        int val = rand() % 10;
        produzione(p, val);

        printf("[PRODUTTORE] Ho prodotto: %d\n", val);

    }

    return NULL;
}

void * thread_consumatore(void * x) {

    MonitorProdCons * p = x;

    for(int i=0; i<NUM_CONSUMAZIONI; i++) {

        int val = consumazione(p);

        printf("[CONSUMATORE] Ho consumato: %d\n", val);
    }

    return NULL;
}


void * thread_conteggio(void * x) {

    MonitorProdCons * p = x;

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

        int n = attendi_consumazioni(p, conteggio);

        consumazioni_effettuate += n;

        consumazioni_da_effettuare -= n;

        printf("[CONTEGGIO] Sono stati consumati in totale %d elementi\n", consumazioni_effettuate);
    }

    return NULL;
}
