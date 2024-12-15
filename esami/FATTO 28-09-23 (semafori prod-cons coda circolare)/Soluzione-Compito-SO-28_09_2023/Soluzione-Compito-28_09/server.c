#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include "header.h"

#define NUM_WORKER 3

static struct Coda * coda;
static int ds_sem;
static pthread_t threads[NUM_WORKER];
static int end = 0;

typedef struct {
	int id;
	Count* count;
} parametri;

void init_buffer(Count* c){
	c->num_prenotazioni = 0;	
	pthread_mutex_init(&(c->mutex),NULL);
	printf("[SERVER] - Count inizializzato...\n");
}

void remove_buffer(Count* c){
	pthread_mutex_destroy(&(c->mutex));
	printf("[SERVER] - Count distrutto...\n");
}


void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	parametri* par = (parametri*) arg;
	Count* c = par->count;
	int id = par->id;
	int num_prenotazioni;
	
	while (!end) {
		printf("[WORKER] in attesa di richiesta %d \n",id);
		num_prenotazioni = consuma(coda, ds_sem);	
		printf("[WORKER] - Ho ricevuto la richiesta di %d prenotazioni \n",num_prenotazioni);
		
		if (num_prenotazioni == -1) {
			end = 1;
			break;
		}
		
		pthread_mutex_lock(&(c->mutex));
		c->num_prenotazioni += num_prenotazioni;
		printf("[WORKER] - num prenotazioni totali aggiornato a %d prenotazioni \n",c->num_prenotazioni);
		pthread_mutex_unlock(&(c->mutex));
	}
    pthread_exit(0);
}



int main(){

	int i,k;
	void *status;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	key_t key_shm;
	key_shm = ftok(".",'m');

	int ds_shm = shmget(key_shm, sizeof(struct Coda), 0);

	if(ds_shm<0) { perror("SHM errore"); exit(1); }

	coda = (struct Coda *) shmat(ds_shm, NULL, 0);

	key_t key_sem;
	key_sem = ftok(".",'s');

	ds_sem = semget(key_sem, 4, 0);

	if(ds_sem<0) { perror("SEM errore"); exit(1); }

	//Inizializzazione struttura locale
	
	Count* count = (Count *) malloc(sizeof(Count));
	init_buffer(count);
	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		printf("[SERVER] - creo thread %d...\n",i);
		param[i].id = i;
		param[i].count = count;
		pthread_create(&threads[i],&attr,worker,(void *) &param[i]);	
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  pthread_join(threads[k], (void **)&status);
	}
	
	remove_buffer(count);
	printf("[SERVER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

