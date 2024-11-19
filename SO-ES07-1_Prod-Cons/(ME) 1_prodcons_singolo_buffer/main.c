#include "sys/shm.h"
#include "sys/sem.h"
#include "procedure.h"
#include <stdlib.h>

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
    semctl(sem_id,MESSAGGIO_DISPONIBILE,0);
    semctl(sem_id,SPAZIO_DISPONIBILE,1);


    
    //figli
    int pidProd=fork();
    if (pidProd==0)
    {
        produttore();
        exit(0);
    }
    
    int pidCons=fork();
    if (pidCons==0)
    {
        consumatore();
        exit(0);
    }
    
    wait(NULL);

    wait(NULL);

    //padre
    return 0;
}