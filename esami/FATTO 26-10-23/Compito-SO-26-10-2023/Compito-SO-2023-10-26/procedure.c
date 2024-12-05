#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void init_monitor(MonitorIO * m) {

    /* TBD: Inizializzare l'oggetto monitor */
    m->num_liberi=DIM;
    m->num_occupati=0;
    for(int i=0; i<DIM; i++) {
        m->stato[i] = LIBERO;
    }
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_attesa,NULL);
    pthread_cond_init(&m->cv_prod,NULL);
    pthread_cond_init(&m->cv_cons,NULL);



}

void end_monitor(MonitorIO * m) {

    /* TBD: Disattivare le variabili membro dell'oggetto monitor */
    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->cv_attesa);
    pthread_cond_destroy(&m->cv_prod);
    pthread_cond_destroy(&m->cv_cons);

}

void produzione(MonitorIO * m, char c) {

    pthread_mutex_lock(&m->mutex);
    

    /* TBD: Porre il thread in sospensione se il vettore di buffer è pieno */
    while (m->num_liberi==0)
    {
        printf("PRODUTTORE: In attesa, vettore pieno...\n");

        pthread_cond_wait(&m->cv_prod,&m->mutex);
        //sospesa  produzione
    }
    
    /* TBD: Ricercare un elemento "i" nello stato LIBERO,
            in base allo schema con vettore di stato       */
    int i=0;
    while(m->stato[i] != LIBERO) {
        i++;
    }
    /*while (i<DIM)
    {
        if (m->stato[i]==LIBERO)
        {
            break;
        }
        i++;
        
    }*/
    m->stato[i]=IN_USO;
    m->num_liberi--;
    pthread_mutex_unlock(&m->mutex);


    printf("PRODUZIONE: inizio produzione vettore[%d]\n", i);
    sleep(1 + (rand() % 3));
    m->vettore[i] = c;
    printf("PRODUZIONE: vettore[%d] <- '%c'\n", i, c);

    pthread_mutex_lock(&m->mutex);

    m->stato[i] = OCCUPATO;
    m->num_occupati++;


    /* TBD: Riattivare un thread consumatore sospeso */
    //signal consumatore
    /* TBD: Riattivare i thread sospesi sulla variabile condition "cv_attesa" */
    pthread_cond_signal(&m->cv_cons);
    pthread_cond_broadcast(&m->cv_attesa);
    pthread_mutex_unlock(&m->mutex);

}

char consumazione(MonitorIO * m) {

    pthread_mutex_lock(&m->mutex);
    /* TBD: Porre il thread in sospensione se il vettore di buffer è vuoto */
    while (m->num_occupati==0)
    {
        printf("CONSUMATORE: In attesa, vettore vuoto...\n");
        pthread_cond_wait(&m->cv_cons,&m->mutex);
        //sospesa  consumazione
    }
    
    /* TBD: Ricercare un elemento "i" nello stato OCCUPATO,
            in base allo schema con vettore di stato       */

   /* while (i<DIM)
    {
        if (m->stato[i]==OCCUPATO)
        {
            break;
        }
        i++;
        
    }*/
    int i=0;

     while(m->stato[i] != OCCUPATO) {
        i++;
    }

    m->stato[i]=IN_USO;
    m->num_occupati--;
    pthread_mutex_unlock(&m->mutex);

    printf("CONSUMAZIONE: inizio consumazione vettore[%d]\n", i);
    sleep(1);
    char c = m->vettore[i];
    printf("CONSUMAZIONE: vettore[%d] -> '%c'\n", i, c);

    pthread_mutex_lock(&m->mutex);
    m->stato[i]=LIBERO;
    m->num_liberi++;

    /* TBD: Riattivare un thread produttore sospeso */
    pthread_cond_signal(&m->cv_prod);
    pthread_mutex_unlock(&m->mutex);
    return c;
}

int attesa(MonitorIO * m) {

    int occupati;
    pthread_mutex_lock(&m->mutex);

    /* TBD: Se il numero di buffer occupati è minore di 3,
     *      sospendere il thread sulla condition variable "cv_attesa" */
   
    while(m->num_occupati < 3) {
        pthread_cond_wait(&m->cv_attesa, &m->mutex);
    }
        occupati = m->num_occupati;


     /* TBD: Restituire in uscita il numero di buffer occupati */
    pthread_mutex_unlock(&m->mutex);

    return occupati;
}
