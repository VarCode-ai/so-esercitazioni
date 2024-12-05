#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "header.h"


int main(int argc, char *argv[]){
	int coda;
	pid_t pid;
	long id_sensore;
        
	if (argc < 2 ) {
		printf("ERRORE: il client deve ricevere il proprio ID\n");
		return -1;
	}
        
	id_sensore = atol(argv[1]);
       
	key_t key;
	key = ftok(".",'k');
    
	coda = msgget(key,0);

	printf("[DEBUG] - sensore id coda %d\n",coda);	
	printf("[Client %d] - invio richieste... con id: %ld \n",getpid(),id_sensore);


	int i = 0;
	Misura misura;	

	srand(getpid()*time(NULL));
	
	// TODO: inviare in cilco NUM_REQ misure da inviare al server tramite coda
	for ( i = 0; i < NUM_REQ; i++)
	{
		misura.misura=rand()%101;
	misura.type=id_sensore;
		msgsnd(coda,&misura,sizeof(Misura)-sizeof(long),IPC_NOWAIT);

	}
	



	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
