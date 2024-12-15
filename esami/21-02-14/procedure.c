#include "header.h"
#include <unistd.h>

void inizializza( monitor_treno * m){
    pthread_mutex_lock(&m->mutex);
    m->stazione=0;
    
    pthread_cond_init(&m->cv_lettori,NULL);
    pthread_cond_init(&m->cv_scrittori,NULL);

}
void rimuovi( monitor_treno * m){

} 

void inizio_lettura(monitor_treno * m){
    pthread_mutex_lock(&m->mutex);
    while (m->num_scrittori>0)
    {
        m->num_lettori_wait++;
        pthread_cond_wait(&m->cv_lettori,&m->mutex);
        m->num_lettori_wait--;
    }
    m->num_lettori++;
        
    pthread_mutex_unlock(&m->mutex);
}
void fine_lettura(monitor_treno * m){
    pthread_mutex_lock(&m->mutex);
   
    m->num_lettori--;
    if (m->num_lettori==0)
    {
        pthread_cond_signal(&m->cv_scrittori);
    }
        
    pthread_mutex_unlock(&m->mutex);
}

void inizio_scrittura(monitor_treno * m){
    pthread_mutex_lock(&m->mutex);
    while (m->num_scrittori>0 || m->num_lettori>0)
    {
        m->num_scrittori_wait++;
        pthread_cond_wait(&m->cv_scrittori,&m->mutex);
        m->num_scrittori_wait--;
    }
    m->num_scrittori++;
        
    pthread_mutex_unlock(&m->mutex);
}
void fine_scrittura(monitor_treno * m){
    pthread_mutex_lock(&m->mutex);
   
    m->num_scrittori--;
    if (m->num_scrittori_wait>0)
    {
        pthread_cond_signal(&m->cv_scrittori);
    }else{
        pthread_cond_broadcast(&m->cv_lettori);
    }
        
    pthread_mutex_unlock(&m->mutex);
}

int leggi_stazione(monitor_treno * m){
    int stazione;
    inizio_lettura(m);
    sleep(1);
    stazione=m->stazione;
    fine_lettura(m);
    return stazione;
    
} 
void scrivi_stazione(monitor_treno * m, int stazione){
    inizio_scrittura(m);
    sleep(1);
    m->stazione=stazione;
    fine_scrittura(m);
}