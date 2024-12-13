#include <pthread.h>
#include "header.h"
#include <stdlib.h>
#include <stdio.h>
void * thread_routine(void * arg){
    parametri *par=(parametri*)arg;
    GestioneVoli *g=(GestioneVoli *) par->m;
    
    int id=par->id;
    InserisciVolo(g,id);
    AggiornaVolo(g,id,1000);
    AggiornaVolo(g,id,2000);
    AggiornaVolo(g,id,1000);
    RimuoviVolo(g,id);

    pthread_exit(NULL);
}

int main(){
    //thread 
    pthread_t threads[NUM_THREADS];

    //monitor 
    GestioneVoli *monitor=malloc(sizeof(GestioneVoli));
    init_monitor(monitor);
    parametri par[NUM_THREADS];

    //crea thread   
    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("[MAIN]: creazione thread %d",i);
        par[i].id=i;
        par[i].m=monitor;
        pthread_create(&threads[i],NULL,thread_routine,&par[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        
        pthread_join(threads[i],NULL);
    }


    

}