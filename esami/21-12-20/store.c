#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include "store.h"

#define NUM_LETTORI 3
#define NUM_SCRITTORI 2

// code di richiesta e risposte
int ds_queue_request, ds_queue_response;

int main(){
    pid_t pid;
    int status, k;

    Magazzino* magazzino;
    
    // Creazione code
    int key_queue_request = ftok(".",'r');// TODO aggiungere codice per la generazione della chiave
    int key_queue_response = ftok(".",'s');// TODO aggiungere codice per la generazione della chiave
    
    // TODO aggiungere codice di inizializzazione delle code
    ds_queue_request = msgget(key_queue_request,IPC_CREAT|0664);
    ds_queue_response = msgget(key_queue_response,IPC_CREAT|0664);
    
    printf("[store] Coda ds_queue_request ID: %d\n", ds_queue_request);
    printf("[store] Coda ds_queue_response ID: %d\n", ds_queue_response);

    // Creazione shared memory del magazzino
    int ds_shm_magazzino;
    int key_shm_magazzino = ftok(".",'m');// TODO aggiungere chiave

    // TODO aggiungere codice per la creazine e l'attach della shared memory
    ds_shm_magazzino = shmget(key_shm_magazzino,sizeof(Magazzino),IPC_CREAT|0664);
    magazzino = shmat(ds_shm_magazzino,NULL,0);

    init_magazzino(magazzino);

    // creazione processi
    for (k = 0; k < NUM_LETTORI; k++) {
        pid = fork();
        if (pid == 0) {                //processo figlio
            lettore(magazzino);
            exit(0); 
        }
    }

     for (k = 0; k < NUM_SCRITTORI; k++) {
        pid = fork();
        if (pid == 0) {                //processo figlio
            scrittore(magazzino);
            exit(0); 
        }
    }

    for (k=0; k<NUM_LETTORI+NUM_SCRITTORI; k++){
        pid = wait(&status);
        if (pid == -1)
            perror("errore");
        else
            printf ("[store] Figlio n.ro %d e\' morto con status= %d \n", pid, status);
    }

    destroy_magazzino(magazzino);
    // TODO aggiungere codice per la rimozione della shared memory
    return 0;

}
