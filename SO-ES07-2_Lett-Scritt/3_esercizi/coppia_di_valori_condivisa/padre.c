#include "sys/shm.h"
#include "sys/sem.h"
#include "unistd.h"
#include "stdio.h"
#include <sys/ipc.h>
#include <stdlib.h>

#include "procedure.h"
#include <sys/wait.h>

int main(){
    printf("padre");
    key_t chiaveshm=ftok(".shm",1);
    key_t chiavesem=ftok(".sem",1);

    //shared memory
    int ds_shm=shmget(chiaveshm,sizeof(Buffer),IPC_CREAT|0664);
    Buffer *buf=shmat(ds_shm,NULL,0);
    //semafori
    int ds_sem=semget(chiavesem,2,IPC_CREAT|0664);
    semctl(ds_sem,MUTEXL,SETVAL,1);
    semctl(ds_sem,SYNCH,SETVAL,1);

    //inizializza risorse
    buf->numlettori=0;
    buf->numscrittori=0;
    buf->val1=0;
    buf ->val2=0;
    //crea processi

   /* for (int i = 0; i < NUM_LETTORI; i++)
    {
        int pid=fork();
        if (pid==0)
        {
            printf("lett");
            execl("./lettore","lettore",i+"",NULL);
            exit(0);
        }
    }*/

    int pidScrittore=fork();
    if (pidScrittore==0)
    {
        execl("./scrittore","scrittore",NULL);
        exit(0);
    }

    wait(NULL);
    //wait(NULL);
    //wait(NULL);
  

    
    
    //rimuovi risorse
}