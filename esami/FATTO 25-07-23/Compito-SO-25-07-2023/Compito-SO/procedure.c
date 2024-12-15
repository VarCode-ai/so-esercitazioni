#include <stdio.h>

#include "procedure.h"

void inizializza(MonitorPortafoglio * m, int quantita_iniziale, int valore_iniziale) {

    /* TBD: Inizializzare l'oggetto monitor */
    m->quantita=quantita_iniziale;
    m->valore=valore_iniziale;
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_attesa,NULL);

}

void termina(MonitorPortafoglio * m) {

    /* TBD: De-inizializzare l'oggetto monitor */
}

void acquisto(MonitorPortafoglio * m, int quantita, int prezzo) {
    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    
    while (prezzo<(m->valore))
    {
        pthread_cond_wait(&m->cv_attesa,&m->mutex);
    }
    
    m->quantita += quantita;
    pthread_mutex_unlock(&m->mutex);
}

void vendita(MonitorPortafoglio * m, int quantita, int prezzo) {

    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    while (prezzo>(m->valore) || quantita>(m->quantita))
    {
        pthread_cond_wait(&m->cv_attesa,&m->mutex);
    }
    
    m->quantita -= quantita;
    pthread_mutex_unlock(&m->mutex);
}

void aggiorna(MonitorPortafoglio * m, int valore) {

    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    m->valore = valore;
    
    pthread_mutex_unlock(&m->mutex);
    pthread_cond_broadcast(&m->cv_attesa);
}

int leggi(MonitorPortafoglio * m) {

    /* TBD: Aggiungere la sincronizzazione */
    pthread_mutex_lock(&m->mutex);
    int valore;

    valore = m->valore;
    pthread_mutex_unlock(&m->mutex);
    return valore;
    
}