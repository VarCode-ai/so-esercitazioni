#include <pthread.h>
#include "procedure.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
void * produttore(void *arg){
        printf("[MAIN] Avvio thread produttore\n");

    Monitor *m=(Monitor*) arg;
    for (int i = 0; i < NUM_PRODUZIONI; i++)
    {
        int val=rand()%10;
        int ret=produci(m,val);
        while (ret<0)
        {
            sleep(1);
           ret= produci(m,val);
            
        }
        
    }
    pthread_exit(NULL);
    
}

void * consumatore(void *arg){

    printf("[MAIN] Avvio thread consumatore\n");

    Monitor *m=(Monitor*)arg;

    for (int i = 0; i < 20; i++)
    {
        int val;
        consuma(m,&val);
        printf("[MAIN] Consumatore preleva: %d\n",val);

        sleep(2);
        //consuma
    }
    
    pthread_exit(NULL);

}




int main (){
     //crea thread
    pthread_t pthreads[NUM_PRODUTTORI+1];
    
     //crea monitor
    printf("[MAIN] inizializza_monitor\n");

    Monitor *monitor =malloc(sizeof(Monitor));
    inizializza_monitor(monitor);
    srand(time(NULL));

     //produci e consuma
    printf("[MAIN] pthread_create\n");

    for (int i = 0; i < NUM_PRODUTTORI+1; i++)
    {
        if (i==0)
        {
            pthread_create(&pthreads[i],NULL,consumatore,monitor);
            //consumatore
        } else{
            //produttore
            pthread_create(&pthreads[i],NULL,produttore,monitor);

        }
        
    }
    for (int i = 0; i < NUM_PRODUTTORI+1; i++)
    {
        pthread_join(pthreads[i],NULL);
    }
    


    
    

    return 0;
}