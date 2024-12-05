#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "procedure.h"

#define QUANTITA_INIZIALE 500
#define VALORE_INIZIALE 100
#define NUM_THREADS 4
void * aggiornatore(void *);
void * azionista(void *);

int main() {

    MonitorPortafoglio *m[3];
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i<3; i++) {
        /* TBD: Creare ed inizializzare gli oggetti monitor */
        m[i] = (MonitorPortafoglio *)malloc(sizeof(MonitorPortafoglio));
        inizializza(m[i],QUANTITA_INIZIALE,VALORE_INIZIALE);
    }

    /* TBD: Creare il thread aggiornatore, passargli il vettore di oggetti monitor */
    pthread_create(&threads[3],NULL,aggiornatore,(void *)m);

    for(int i = 0; i<3; i++) {
        pthread_create(&threads[i],NULL,azionista,(void *)m[i]);
        /* TBD: Creare i thread azionisti, passando a ognuno uno degli oggetti monitor */
    }

    pthread_join(threads[3],NULL);
    /* TBD: Attendere la terminazione del thread aggiornatore */

    for(int i = 0; i<3; i++) {
    pthread_join(threads[i],NULL);
        /* TBD: Attendere la terminazione dei thread azionisti, deallocare gli oggetti */
    }


    return 0;
}



void * aggiornatore(void * arg) {

    /* TBD: Gestire il passaggio dei parametri

        Suggerimento: Dichiarare l'array di oggetti con la sintassi "MonitorPortafoglio ** m = ..." 
    */
   MonitorPortafoglio **m=(MonitorPortafoglio **) arg;
    for(int i=0; i<10; i++) {

        for(int j=0; j<3; j++) {

            int valore = 100 + ((rand()%21) - 10);
            printf("[AGGIORNATORE] Aggiornamento titolo %d, valore %d\n", j, valore);
            aggiorna(m[j],valore);
            
            /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor */
        }

        sleep(1);
    }

    printf("[AGGIORNATORE] Terminazione\n");

    for(int j=0; j<3; j++) {

        /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor, con valore 200 */
        aggiorna(m[j],200);
        sleep(1);
        aggiorna(m[j],0);
        /* TBD: Invocare il metodo "aggiorna" sull'oggetto monitor, con valore 0 */
    }

    pthread_exit(NULL);
    return NULL;
}


void * azionista(void * m) {

    /* TBD: Gestire il passaggio dei parametri */
    

    printf("[AZIONISTA] Richiesta di vendita azioni, prezzo offerto 105\n");

    /* TBD: Invocare il metodo "vendita" sull'oggetto monitor, con valore 105 e quantità 50 */
    vendita(m,50,105);

    int valore = leggi(m)/* TBD: Invocare il metodo "leggi" */;

    printf("[AZIONISTA] Vendita effettuata, valore attuale %d\n", valore);


    printf("[AZIONISTA] Richiesta di acquisto azioni, prezzo offerto 95\n");

    /* TBD: Invocare il metodo "acquisto" sull'oggetto monitor, con valore 95 e quantità 50 */
    acquisto(m,50,95);
    valore = leggi(m);/* TBD: Invocare il metodo "leggi" */;

    printf("[AZIONISTA] Acquisto effettuato, valore attuale %d\n", valore);


    printf("[AZIONISTA] Terminazione\n");
   // pthread_exit(NULL);

    return NULL;
}