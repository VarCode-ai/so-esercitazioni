#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void init_monitor(MonitorIO * m) {

    pthread_mutex_init(&m->mutex, NULL);
    pthread_cond_init(&m->cv_produttori, NULL);
    pthread_cond_init(&m->cv_consumatori, NULL);
    pthread_cond_init(&m->cv_attesa, NULL);

    m->num_liberi = DIM;
    m->num_occupati = 0;

    for(int i=0; i<DIM; i++) {
        m->stato[i] = LIBERO;
    }
}

void end_monitor(MonitorIO * m) {

    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->cv_produttori);
    pthread_cond_destroy(&m->cv_consumatori);
    pthread_cond_destroy(&m->cv_attesa);
}

/////////////////// PROD-CONS VETTORE DI STATO CON THREADS /////////////////////
void produzione(MonitorIO * m, char c) {

    pthread_mutex_lock(&m->mutex);
    while(m->num_liberi == 0) {

        printf("PRODUTTORE: In attesa, vettore pieno...\n");
        pthread_cond_wait(&m->cv_produttori, &m->mutex);
    }

    int i = 0;
    while(m->stato[i] != LIBERO) {
        i++;
    }
    m->stato[i] = IN_USO;
    m->num_liberi--;
    pthread_mutex_unlock(&m->mutex);


    printf("PRODUZIONE: inizio produzione vettore[%d]\n", i);
    sleep(1 + (rand() % 3));
    m->vettore[i] = c;
    printf("PRODUZIONE: vettore[%d] <- '%c'\n", i, c);


    pthread_mutex_lock(&m->mutex);
    m->stato[i] = OCCUPATO;
    m->num_occupati++;

    pthread_cond_signal(&m->cv_consumatori);
    pthread_cond_broadcast(&m->cv_attesa);

    pthread_mutex_unlock(&m->mutex);
}

char consumazione(MonitorIO * m) {

    pthread_mutex_lock(&m->mutex);
    while(m->num_occupati == 0) {
        printf("PRODUTTORE: In attesa, vettore vuoto...\n");
        pthread_cond_wait(&m->cv_consumatori, &m->mutex);
    }

    int i = 0;
    while(m->stato[i] != OCCUPATO) {
        i++;
    }
    m->stato[i] = IN_USO;
    m->num_occupati--;
    pthread_mutex_unlock(&m->mutex);


    printf("CONSUMAZIONE: inizio consumazione vettore[%d]\n", i);
    sleep(1);
    char c = m->vettore[i];
    printf("CONSUMAZIONE: vettore[%d] -> '%c'\n", i, c);



    pthread_mutex_lock(&m->mutex);
    m->stato[i] = LIBERO;
    m->num_liberi++;
    pthread_cond_signal(&m->cv_produttori);
    pthread_mutex_unlock(&m->mutex);


    return c;
}

int attesa(MonitorIO * m) {

    int occupati;

    pthread_mutex_lock(&m->mutex);

    while(m->num_occupati < 3) {
        pthread_cond_wait(&m->cv_attesa, &m->mutex);
    }

    occupati = m->num_occupati;

    pthread_mutex_unlock(&m->mutex);

    return occupati;
}