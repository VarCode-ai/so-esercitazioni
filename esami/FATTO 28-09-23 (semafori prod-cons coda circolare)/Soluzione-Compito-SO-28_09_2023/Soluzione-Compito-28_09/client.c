#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


int main(){
	key_t key_shm;
	key_shm = ftok(".",'m');

	int ds_shm = shmget(key_shm, sizeof(struct Coda), IPC_CREAT|0664);

	if(ds_shm<0) { perror("SHM errore"); exit(1); }

	struct Coda * c;

	c = (struct Coda *) shmat(ds_shm, NULL, 0);

	key_t key_sem;
	key_sem = ftok(".",'s');

	int ds_sem = semget(key_sem, 4, IPC_CREAT|0664);

	if(ds_sem<0) { perror("SEM errore"); exit(1); }
    
	printf("[Client %d] - invio richieste...\n",getpid());

	int i = 0;
	int num_prenotazioni;	

	srand(time(NULL));

	while(i < NUM_REQ){
		num_prenotazioni = rand()%5+1;
		// Invio di una richiesta
		printf("[Client %d] - Invio richiesta {%d} al server...\n",getpid(),num_prenotazioni);
		produci(c,ds_sem,num_prenotazioni);
		sleep(1);
		i++;
	}

	produci(c,ds_sem,-1);
	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
