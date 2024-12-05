#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "header.h"

void * produttore(void *);
void * consumatore(void *);

int main() {

    pthread_t thread_consumatore;
    pthread_t thread_produttori[3];

    /* TBD: Creare un oggetto monitor, e inizializzarlo con init_monitor() */
    MonitorIO *monitor=malloc(sizeof(MonitorIO));
    init_monitor(monitor);

    /* TBD: Creare 3 thread produttori, chiamando la funzione produttore() */
    for (int i = 0; i < NUM_PRODUTTORI; i++)
    {
        pthread_create(&thread_produttori[i],NULL,produttore,(void *) monitor);
    }

    /* TBD: Creare 1 thread consumatore, chiamando la funzione consumatore() */
    pthread_create(&thread_consumatore,NULL,consumatore,(void *) monitor);


    /* TBD: Attendere la terminazione dei 3 thread produttori */
    for (int i = 0; i < NUM_PRODUTTORI; i++)
    {
        pthread_join(thread_produttori[i],NULL);
    }
    /* TBD: Attendere la terminazione del thread consumatore */
    pthread_join(thread_consumatore,NULL);

    /* TBD: Disattivare e deallocare l'oggetto monitor */
    end_monitor(monitor);
    free(monitor);
}


void * produttore(void * p) {

    printf("Avvio thread produttore...\n");
    MonitorIO *m = (MonitorIO*) p;

    for(int i=0; i<5; i++) {

        sleep(1);

        // genera carattere casuale tra "a" e "z"
        char carattere = 97 + (rand() % 26);
        produzione(m,carattere);
        /* TBD: Chiamare il metodo "produzione()", passando l'oggetto monitor e
         *      la variabile "carattere" */
    }

    return NULL;
}

void * consumatore(void * p) {

    printf("Avvio thread consumatore...\n");


    int totale_consumazioni = 0;
    MonitorIO *m=(MonitorIO*) p;
    while(totale_consumazioni < 15) {

        int occupati = 1;
        
        if(totale_consumazioni < 13) {

            printf("CONSUMATORE: In attesa di elementi disponibili...\n");

            occupati = attesa(m);/* TBD: Chiamare il metodo "attesa()" */
            
            printf("CONSUMATORE: %d elementi disponibili\n", occupati);
        }

        for(int j=0; j<occupati; j++) {

            char carattere;
            
            carattere = consumazione(m);/* TBD: Chiamare il metodo "consumazione()" */
        }

        totale_consumazioni += occupati;
    }

    return NULL;
}
