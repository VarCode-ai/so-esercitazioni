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

// struttura per passaggio parametri ai thread
typedef struct {
	int id;
	Count* count;
} parametri;

void init_buffer(Count* c){
	//TODO aggiungere codice per inizializzare Count
	c->num_prenotazioni=0;
	pthread_mutex_init(&c->mutex,NULL);

}

void remove_buffer(Count* c){
	//TODO aggiungere codice per rimuovere Count
	pthread_mutex_destroy(&c->mutex);
}


void *worker(void* arg){

	

	//TODO: completare il passaggio dei parametri
	parametri* par = (parametri*) arg;
	printf("[WORKER %d] - start del thread...\n",par->id);
	while (!end) {
		//TODO completare il codice del worker
		// worker preleva dalla coda con funzione "consuma"
		printf("[WORKER] in attesa di richiesta %d \n",par->id);
		int val=consuma(coda,ds_sem);
		printf("[WORKER] - Ho ricevuto la richiesta di %d prenotazioni \n",val);

		// aggiornamento (eventuale) variabile end se valore ricevuto = -1
		if (val==-1)
		{
			printf("[WORKER %d] - ho ottenuto -1\n",par->id);
			end=1;
			break;
		}
		
		// aggiornamento Count in mutua esclusione	
		pthread_mutex_lock(&(par->count->mutex));
		par->count->num_prenotazioni+=val;
		printf("[WORKER %d] - aggiorno counter num_prenotazioni:%d\n",par->id,par->count->num_prenotazioni);
		pthread_mutex_unlock(&(par->count->mutex));
	}
	//TODO uscita dal thread
	printf("[WORKER %d] - esco\n",par->id);
	pthread_exit(0);
}



int main(){

	int i,k;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok(".",'m');

	int ds_shm = shmget(key_shm,sizeof(struct Coda),0);
	if(ds_shm<0) { perror("SHM errore"); exit(1); }
	
	coda = (struct Coda *) shmat(ds_shm,NULL,0);
	
	//ottenimento array semafori

	key_t key_sem;
	key_sem = ftok(".",'s');

	ds_sem = semget(key_sem,4,0);
	if(ds_sem<0) { perror("SEM errore"); exit(1); }
	//TODO Creazione e Inizializzazione struttura locale
	Count* count =(Count *) malloc(sizeof(Count));
	init_buffer(count);
	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		//TODO creazione threads
		param[i].count=count;
		param[i].id=i;
		printf("[SERVER] - creo thread %d...\n",i);
		pthread_create(&threads[i],&attr,worker,(void *) &param[i]);
			
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  //TODO join 
	  pthread_join(threads[k],NULL);
	}
	
	//TODO rimozione struttura locale
	printf("[SERVER] - bye bye...\n");
	remove_buffer(count);
	
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

