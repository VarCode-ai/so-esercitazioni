#include "sys/shm.h"
#include "sys/sem.h"
#include "stdio.h"
#include <unistd.h>
#include <stdlib.h>

#include "semafori.h"
#include "procedure.h"

int main(int argc, char *argv[]){
    printf("lettore");
    if (argc < 2) {
        printf("Errore: specificare il numero del lettore (1 o 2)\n");
        return 1;
    }
    
    int chiaveshm=ftok(".shm",1);
    int chiavesem=ftok(".sem",1);

    int ds_shm=shmget(chiaveshm,sizeof(Buffer),0);
    Buffer *buf=shmat(chiaveshm,NULL,0);
    int ds_sem=semget(chiavesem,2,0);
    
    
    

    int lettore_id = atoi(argv[1]);
    for (int i = 0; i < 5; i++)
    {
        inizioLettura(buf,ds_sem);
        printf("lettore");

        sleep(2);
        int val;
        if (lettore_id==0){
            val=buf->val1;
        }
        else{
            val=buf->val2;
        }
        printf("Valore letto da PID: %d con indice %d è %d:",getpid(),lettore_id,val);
        
        fineLettura(buf,ds_sem);
    }
    
    
}