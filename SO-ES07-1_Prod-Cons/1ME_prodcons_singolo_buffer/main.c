#include "sys/shm.h"
#include "sys/sem.h"
#include "procedure.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int main(){

    //shared memory
    key_t chiaveshm=IPC_PRIVATE;
    int shm_id=shmget(chiaveshm,sizeof(int),IPC_CREAT|0664);
    if(shm_id<0) { perror("SHM errore"); exit(1); }
	int * p;

	p = (int *) shmat(shm_id, NULL, 0);

    ///semafori
    key_t chiavesem=IPC_PRIVATE;
    int sem_id=semget(chiavesem,2,IPC_CREAT|0644);
    if(sem_id<0) { perror("SEM errore"); exit(1); }

    *p =0;
    semctl(sem_id,MESSAGGIO_DISPONIBILE,SETVAL,0);
    semctl(sem_id,SPAZIO_DISPONIBILE,SETVAL,1);


    
    //figli
    int pidCons=fork();
    if (pidCons==0)
    {		
        printf("Inizio figlio consumatore\n");
        consumatore(p,sem_id);
        exit(0);
    }
    

    int pidProd=fork();
    if (pidProd==0)
    {
        printf("Inizio figlio produttore\n");
        produttore(p,sem_id);
        exit(0);
    }
    
    
    wait(NULL);
    printf("Primo figlio terminato\n");

    wait(NULL);
    printf("Secondo figlio terminato\n");

    shmctl(shm_id,IPC_RMID,NULL);
    semctl(sem_id,IPC_RMID,0);
    
    //padre
    return 0;
}