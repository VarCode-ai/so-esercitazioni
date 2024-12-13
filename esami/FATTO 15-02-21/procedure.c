#include "procedure.h"
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
void * timer (void *arg){
    printf("[MONITOR] Avvio thread timer\n");

    Monitor *monitor=(Monitor *) arg;
    while (1)
    {
        int uscita;
        sleep(1);

        pthread_mutex_lock(&monitor->mutex);
        printf("[MONITOR] Thread timer\n");
        pthread_cond_broadcast(&monitor->cv_prod);
        pthread_cond_broadcast(&monitor->cv_cons);
        uscita=monitor->uscita_timer;
        pthread_mutex_unlock(&monitor->mutex);
        if (uscita!=0)
        {
            break;
        }
    }

    printf("[MONITOR] Uscita thread timer\n");
    pthread_exit(NULL);
    
}


int produci(Monitor *monitor, int val){
    pthread_mutex_lock(&monitor->mutex);

    struct timeval t1;
    gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;

    while (monitor->count==DIM_BUFFER)
    {
        printf("[MONITOR] Produttore in attesa...\n");

        pthread_cond_wait(&monitor->cv_prod,&monitor->mutex);
        struct timeval t2;
	    gettimeofday(&t2,NULL);
        long current_time = t2.tv_sec*1000000 + t2.tv_usec;

        if(current_time - start_time >= 3000000) {

            printf("[MONITOR] Produttore ha atteso oltre 3 secondi, esce\n");

            pthread_mutex_unlock(&monitor->mutex);
            return -1;
        }
    }

    monitor->buf[monitor->testa]=val;
    monitor->testa=(monitor->testa+1)%DIM_BUFFER;
    monitor->count++;

    printf("[MONITOR] Produzione %d\n", val);
    pthread_cond_signal(&monitor->cv_cons);
    pthread_mutex_unlock(&monitor->mutex);
    
    return 1;


    
}

int consuma(Monitor *m,int *val){
    pthread_mutex_lock(&(m->mutex));
    struct timeval t1;
    gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;

    while (m->count==0)
    {
        printf("[MONITOR] Consumatore in attesa...\n");

        pthread_cond_wait(&m->cv_cons,&m->mutex);
        struct timeval t2;
	    gettimeofday(&t2,NULL);
        long current_time = t2.tv_sec*1000000 + t2.tv_usec;

        if(current_time - start_time >= 3000000) {

            printf("[MONITOR] Consumatore ha atteso oltre 3 secondi, esce\n");

            pthread_mutex_unlock(&m->mutex);
            return -1;
        }
    }

    *val=m->buf[m->coda];
    m->coda=(m->coda+1)%DIM_BUFFER;
    m->count--;

    printf("[MONITOR] Consumazione %d\n", *val);
    pthread_cond_signal(&m->cv_prod);
    pthread_mutex_unlock(&m->mutex);
    return 1;
}

void inizializza_monitor(Monitor * monitor){
    
    monitor->coda=0;
    monitor->testa=0;
    monitor->uscita_timer=0;
    monitor->count=0;

    for (int i = 0; i < DIM_BUFFER; i++)
    {
        monitor->buf[i]=0;
    }
    pthread_t t;

    pthread_mutex_init(&(monitor->mutex),NULL);
    pthread_cond_init(&monitor->cv_cons,NULL);
    pthread_cond_init(&monitor->cv_prod,NULL);
    pthread_create(&t, NULL, timer, monitor);

}