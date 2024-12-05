#include <stdio.h>

#include "procedure.h"

void inizializza(MonitorPortafoglio * m, int quantita_iniziale, int valore_iniziale) {
    m->quantita=quantita_iniziale;
    m->valore=valore_iniziale;

    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_wait_vendita,NULL);
    pthread_cond_init(&m->cv_wait_acquista,NULL);

    /* TBD: Inizializzare l'oggetto monitor */
}

void termina(MonitorPortafoglio * m) {

    /* TBD: De-inizializzare l'oggetto monitor */
}

void acquisto(MonitorPortafoglio * m, int quantita, int prezzo) {
    pthread_mutex_lock(&m->mutex);
    /* TBD: Aggiungere la sincronizzazione */
    while (prezzo<m->valore)
    {
        pthread_cond_wait(&m->cv_wait_acquista,&m->mutex);
        //wait_acquisto()
    }
    
    m->quantita += quantita;
    pthread_mutex_unlock(&m->mutex);

}

void vendita(MonitorPortafoglio * m, int quantita, int prezzo) {
    pthread_mutex_lock(&m->mutex);
    /* TBD: Aggiungere la sincronizzazione */
    while (prezzo>m->valore || quantita>m->quantita)
    {
        pthread_cond_wait(&m->cv_wait_vendita,&m->mutex);
    }
    
    m->quantita -= quantita;
    pthread_mutex_unlock(&m->mutex);

}

void aggiorna(MonitorPortafoglio * m, int valore) {
    pthread_mutex_lock(&m->mutex);
    /* TBD: Aggiungere la sincronizzazione */
    m->valore = valore;
    pthread_cond_signal(&m->cv_wait_acquista);
    pthread_cond_signal(&m->cv_wait_vendita);
    pthread_mutex_unlock(&m->mutex);
}

int leggi(MonitorPortafoglio * m) {
    pthread_mutex_lock(&m->mutex);
    /* TBD: Aggiungere la sincronizzazione */
    int valore;
    valore = m->valore;
    pthread_mutex_unlock(&m->mutex);
    return valore;
}