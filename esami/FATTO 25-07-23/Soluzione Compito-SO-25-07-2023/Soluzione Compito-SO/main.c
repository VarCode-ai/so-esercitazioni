#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "procedure.h"

#define QUANTITA_INIZIALE 500
#define VALORE_INIZIALE 100

void * aggiornatore(void *);
void * azionista(void *);

int main() {

    MonitorPortafoglio * m[3];

    for(int i = 0; i<3; i++) {

        m[i] = malloc(sizeof(MonitorPortafoglio));

        inizializza(m[i], QUANTITA_INIZIALE, VALORE_INIZIALE);
    }


    pthread_t thread_aggiornatore;

    pthread_create(&thread_aggiornatore, NULL, aggiornatore, (void *) m);


    pthread_t thread_azionista[3];

    for(int i = 0; i<3; i++) {
            
            pthread_create(&thread_azionista[i], NULL, azionista, (void *) m[i]);
    }


    pthread_join(thread_aggiornatore, NULL);

    for(int i = 0; i<3; i++) {

        pthread_join(thread_azionista[i], NULL);

        termina(m[i]);

        free(m[i]);
    }


    return 0;
}



void * aggiornatore(void * p) {

    MonitorPortafoglio ** m = p;

    for(int i=0; i<10; i++) {

        for(int j=0; j<3; j++) {

            int valore = 100 + ((rand()%21) - 10);

            printf("[AGGIORNATORE] Aggiornamento titolo %d, valore %d\n", j, valore);

            aggiorna(m[j], valore);
        }

        sleep(1);
    }

    printf("[AGGIORNATORE] Terminazione\n");

    for(int j=0; j<3; j++) {

        aggiorna(m[j], 200);

        sleep(1);

        aggiorna(m[j], 0);
    }

    return NULL;
}


void * azionista(void * p) {

    MonitorPortafoglio * m = p;

    printf("[AZIONISTA] Richiesta di vendita azioni, prezzo offerto 105\n");

    vendita(p, 50, 105);

    int valore = leggi(p);

    printf("[AZIONISTA] Vendita effettuata, valore attuale %d\n", valore);


    printf("[AZIONISTA] Richiesta di acquisto azioni, prezzo offerto 95\n");

    acquisto(p, 50, 95);

    valore = leggi(p);

    printf("[AZIONISTA] Acquisto effettuato, valore attuale %d\n", valore);


    printf("[AZIONISTA] Terminazione\n");

    return NULL;
}