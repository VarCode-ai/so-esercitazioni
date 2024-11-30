#include "header.h"
#include "semafori.h"
#include "procedure.h"
#include "sys/shm.h"
#include "sys/sem.h"
#include "stdio.h"
#include <unistd.h>
 #include <stdlib.h>

int main(){

    printf("scrittore");
    int chiaveshm=ftok(".shm",1);
    int chiavesem=ftok(".sem",1);

    int ds_shm=shmget(chiaveshm,sizeof(Buffer),0);
    Buffer *buf=shmat(chiaveshm,NULL,0);
    int ds_sem=semget(chiavesem,2,0);

    srand(getpid());

    for (int i = 0; i < 5; i++)
    {
        //inizioScrittura(ds_sem);
        sleep(1);    
        int val1= rand()%10;
        int val2= rand()%10;

        buf-> val1 = val1;
        buf->val2=val2;
        printf("Val 1 scritto da PID: %d è %d:",getpid(),val1);
        printf("Val 2 scritto da PID: %d è %d:",getpid(),val2);

        //fineScrittura(ds_sem);
    
    }
    
    
}