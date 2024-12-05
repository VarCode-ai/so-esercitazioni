#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	key_t key_shm;
	key_shm = ftok(".",'m');

	int ds_shm = shmget(key_shm, sizeof(struct Coda), IPC_CREAT|0664);

	if(ds_shm<0) { perror("SHM errore"); exit(1); }

	struct Coda * c;

	c = (struct Coda *) shmat(ds_shm, NULL, 0);
	c->testa = 0;
	c->coda = 0;

	key_t key_sem;
	key_sem = ftok(".",'s');

	int ds_sem = semget(key_sem, 4, IPC_CREAT|0664);

	if(ds_sem<0) { perror("SEM errore"); exit(1); }

    semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFFER);
    semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, 0);
    semctl(ds_sem, MUTEX_C, SETVAL, 1);
    semctl(ds_sem, MUTEX_P, SETVAL, 1);

	int i = 0;
	int pid;
	int status;
	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		sleep(1);

		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				printf("[SERVER %d] - Sono stato creato...\n",getpid());
				execl("./server","./server",NULL);
				exit(-1);
			}else{

				printf("[CLIENT %d] - Sono stato creato...\n",getpid());
				execl("./client","./client",NULL);
				exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	shmctl(ds_shm, IPC_RMID, NULL);
    semctl(ds_sem, 0, IPC_RMID);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}
