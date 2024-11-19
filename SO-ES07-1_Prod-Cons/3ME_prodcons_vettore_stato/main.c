#include "stdio.h"
#include "unistd.h"
#include "sys/shm.h"
#include "sys/sem.h"
#include "procedure.h"
#include <stdlib.h>
#include "sys/wait.h"
#include <time.h>

int main(){
    key_t shm_key=IPC_PRIVATE;
    key_t sem_key=IPC_PRIVATE;

    int ds_shm=shmget(shm_key,sizeof(prodcons),IPC_CREAT|0664);
    prodcons *p=shmat(ds_shm,NULL,0);


    int ds_sem=semget(sem_key,4,IPC_CREAT|0664);
    semctl(ds_sem,MUTEXP,SETVAL,1);
    semctl(ds_sem,MUTEXC,SETVAL,1);

    semctl(ds_sem,SPAZIO_DISPONIBILE,SETVAL,DIM);
    semctl(ds_sem,MESSAGGIO_DISPONIBILE,SETVAL,0);

    for (int i = 0; i < NUM_CONS; i++)
    {
        int pid=fork();
        if (pid==0)
        {
            srand(getpid()*time(NULL));
                if (i<=3)
                {
                    printf("porcesso CONSUMATORE lento, PID: %d\n",getpid());
                    consumatore(p,ds_sem,i);
                    exit(0);
                }
                else{
                    printf("porcesso CONSUMATORE veloce, PID: %d\n",getpid());
                    consumatore(p,ds_sem,0);
                    exit(0);
                }
                
        }
    }

    for (int i = 0; i < NUM_PROD; i++)
    {
        int pid=fork();
        if (pid==0)
        {
            srand(getpid()*time(NULL));
                if (i<=3)
                {
                    printf("porcesso PRODUTTORE lento, PID: %d\n",getpid());
                    produttore(p,ds_sem,i);
                    exit(0);
                }
                else{
                    printf("porcesso PRODUTTORE veloce, PID: %d\n",getpid());
                    produttore(p,ds_sem,0);
                    exit(0);
                }
                
        }
    }
    
    for (int i = 0; i < NUM_CONS+NUM_PROD; i++)
    {
        int pid=wait(NULL);
        printf("processo con PID: %d è terminato\n",pid);
    }

    shmctl(ds_shm,IPC_RMID,NULL);
    semctl(ds_sem,0,IPC_RMID);
    
    
    return 0;
}