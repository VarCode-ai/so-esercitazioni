#include "header.h"
#include "pthread.h"

#include <stdlib.h>
#include <unistd.h>
void * t_capitreno(void * arg){
    monitor_treno *m=(monitor_treno * )arg;
    for (int i = 0; i < 10; i++)
    {
        scrivi_stazione(m,i);
        sleep(3);
    }
    pthread_exit(NULL);
    
}
void * t_lettore(void * arg){
    monitor_treno *m=(monitor_treno*) arg;
    for (int i = 0; i < 3; i++)
    {
        leggi_stazione(m);
        sleep((rand()%6)+1);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t threads_capitreni[NUM_CAPITRENO];
    pthread_t threads_lettori[NUM_LETTORI];

    monitor_treno *treni[NUM_CAPITRENO]; 
    for (int i = 0; i < NUM_CAPITRENO; i++)
    {
        treni[i]=malloc(sizeof(monitor_treno)*NUM_CAPITRENO);
        inizializza(treni[i]);
    }
    
    

    for (int i = 0; i < NUM_CAPITRENO; i++)
    {
        inizializza(treni[i]);
        pthread_create(&threads_capitreni[i],NULL,t_capitreno,treni[i]);
    }

    for (int i = 0; i < NUM_LETTORI; i++)
    {
        int numTreno=rand()%4;
        
        pthread_create(&threads_lettori[i],NULL,t_lettore,(void*) treni[numTreno]);
    }

    for (int i = 0; i < NUM_CAPITRENO; i++)
    {
        pthread_join(threads_capitreni[i],NULL);
    }
    
    for (int i = 0; i < NUM_LETTORI; i++)
    {
        pthread_join(threads_lettori[i],NULL);
    }

    


    
    
}