#include "header.h"

//////////// LETT-SCRITT STARVATION ENTRAMBI THREAD ////////////
void inizializza(struct monitor* m){
	m->molo=0;
	m->id_nave=0;

    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->ok_lett_cv,NULL);
    pthread_cond_init(&m->ok_scritt_cv,NULL);
}

void rimuovi (struct monitor* m){
    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->ok_lett_cv);
    pthread_cond_destroy(&m->ok_scritt_cv);
}


void scrittura(struct monitor* m, int molo){
    
    pthread_mutex_lock(&m->mutex);

    while (m->num_lettori>0 || m->num_scrittori>0)
    {
        m->num_scrittori_wait++;
        pthread_cond_wait(&m->ok_scritt_cv,&m->mutex);
        m->num_scrittori_wait--;
    }
    m->num_scrittori++;

    pthread_mutex_unlock(&m->mutex);



    sleep(2);
    m->molo=molo;
    


    pthread_mutex_lock(&m->mutex);

    m->num_scrittori--;
    if (m->num_scrittori_wait>0)
    {
        pthread_cond_signal(&m->ok_scritt_cv);
    }
    else{
        pthread_cond_broadcast(&m->ok_lett_cv);
    }

    pthread_mutex_unlock(&m->mutex);
}


int lettura(struct monitor* m){
    
    pthread_mutex_lock(&m->mutex);

    int posizione;
    while (m->num_scrittori>0)
    {
        m->num_lettori_wait++;
        pthread_cond_wait(&m->ok_lett_cv,&m->mutex);
        m->num_lettori_wait--;
    }
    m->num_lettori++;

    pthread_mutex_unlock(&m->mutex);



    sleep(2);
    posizione=m->molo;



    pthread_mutex_lock(&m->mutex);

    m->num_lettori--;
    if (m->num_lettori==0)
    {
        pthread_cond_signal(&m->ok_scritt_cv);
    }

    pthread_mutex_unlock(&m->mutex);
    return posizione;
}

