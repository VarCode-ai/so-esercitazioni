#include "sys/shm.h"
#include "sys/sem.h"
#include "procedure.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>




bool isInArray(int pid, int array[])
{
    bool isPresent=false;
    for (int i = 0; i < 8; i++)
    {
        if (array[i] == pid)
        {
            isPresent = true;
            break;
        }
    }
    return isPresent;
}


int main()
{
    // shared memory
    key_t shm_key = IPC_PRIVATE;
    key_t sem_key = IPC_PRIVATE;

    int ds_shm = shmget(shm_key, sizeof(prodcons), IPC_CREAT | 0664);
    prodcons *p = shmat(ds_shm, NULL, 0);
    p->testa = 0;
    p->coda = 0;
    // semafori
    int ds_sem = semget(sem_key, 4, IPC_CREAT | 0664);
    semctl(ds_sem, SPAZIO_DISP, SETVAL, DIM);
    semctl(ds_sem, NUM_MESS, SETVAL, 0);

    semctl(ds_sem, MUTEX_P, SETVAL, 1);
    semctl(ds_sem, MUTEX_C, SETVAL, 1);

    // processi
    // prova per vedere se i processi lenti penalizzano i veloci (VERO 2-3 PROCESSI CONSUMATORI HANNO SOFFERTO DI STARVATION)
int *produttoriVeloci = shmat(shmget(IPC_PRIVATE, sizeof(int) * 8, IPC_CREAT | 0664), NULL, 0);
int *consumatoriVeloci = shmat(shmget(IPC_PRIVATE, sizeof(int) * 8, IPC_CREAT | 0664), NULL, 0);
for (int i = 0; i < 8; i++) {
    produttoriVeloci[i] = -1;
    consumatoriVeloci[i] = -1;
}

    for (int i = 0; i < NUM_CONSUMATORI; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            srand(getpid() * time(NULL));
            if (i <= 7)
            {
                __pid_t ppidTemp=getpid();
                printf("porcesso CONSUMATORE veloce, PID: %d\n", ppidTemp);
                consumatoriVeloci[i] = ppidTemp;
                consumatore(p, ds_sem, 0);
                exit(0);
            }
            else
            {
                printf("porcesso CONSUMATORE lento, PID: %d\n", getpid());
                consumatore(p, ds_sem, i & 4);
                exit(0);
            }
        }
    }

    for (int i = 0; i < NUM_PRODUTTORI; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            srand(getpid() * time(NULL));
            if (i <= 7)
            {
                __pid_t pidTemp=getpid();
                produttoriVeloci[i] = pidTemp;
                printf("porcesso PRODUTTORE veloce, PID: %d\n",pidTemp);
                produttore(p, ds_sem, 0);
                exit(0);
            }
            else
            {
                printf("porcesso PRODUTTORE lento, PID: %d\n", getpid());
                produttore(p, ds_sem, i % 4);
                exit(0);
            }
        }
    }

    /*for (int i = 0; i < NUM_CONSUMATORI+NUM_PRODUTTORI; i++)
    {
        int pid=fork();
        if (pid==0)
        {
            srand(getpid()*time(NULL));
            if (i<NUM_CONSUMATORI){
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
            else {
                if (i<=8)
                {
                    printf("porcesso PRODUTTORE lento, PID: %d\n",getpid());
                    produttore(p,ds_sem,i);
                    exit(0);
                }else{
                    printf("porcesso PRODUTTORE veloce, PID: %d\n",getpid());
                    produttore(p,ds_sem,0);
                    exit(0);
                }

            }
        }



    }*/

    /*int pidCons=fork();
    if (pidCons==0)
    {
       int valore= consumatore(p,ds_sem);
       printf("PID CONSUAMTORE: %d \n",getpid());
       exit(0);
    }

    int pidProd=fork();
    if (pidProd==0)
    {
        produttore(p,ds_sem);
        printf("PID PRODUTTORE: %d \n",getpid());
        exit(0);

    }*/
    for (int i = 0; i < NUM_CONSUMATORI + NUM_PRODUTTORI; i++)
    {
        int pidd = wait(NULL);
        if (isInArray(pidd, produttoriVeloci) || isInArray(pidd, consumatoriVeloci))
        {
            printf("Il processo VELOCE con PID: %d è terminato \n", pidd);
        }
        else
        {
            printf("Il processo LENTO con PID: %d è terminato \n", pidd);
        }
    }

    shmctl(ds_shm, IPC_RMID, NULL);
    semctl(ds_sem, 0, IPC_RMID);

    return 0;
}
