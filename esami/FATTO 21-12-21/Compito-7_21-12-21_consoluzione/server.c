#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

#define NUM_WORKER 3

static int coda;
static pthread_t threads[NUM_WORKER+1];


// struttura per il passaggio dei parametri ai thread worker
typedef struct {
	int id;
	Buffer* b;
} parametri;

void init_buffer(Buffer* b){
	//TODO inizializzazione del buffer
	b->somma=0;
	b->messaggi=0;
	pthread_mutex_init(&b->mutex,NULL);
	pthread_cond_init(&b->cv_collector,NULL);
	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
	//TODO distruzione delle variabili de buffer
	
	printf("[SERVER] - Buffer distrutto...\n");
}


void aggiorna_somma(Buffer* b, int somma_parziale){
	// TODO aggiungere la somma parziale alla somma nel buffer,
	//		risvegliando il prelievo della somma finale quando necessario...
	pthread_mutex_lock(&b->mutex);
	printf("[WORKER] - sto aggiornando somma: %d",somma_parziale);
	b->somma+=somma_parziale;
	b->messaggi++;
	if(b->messaggi>=NUM_REQ*NUM_WORKER){
		pthread_cond_signal(&b->cv_collector);
	}
	pthread_mutex_unlock(&b->mutex);

}

int preleva_somma_finale(Buffer* b){
	int somma_finale;
	//TODO prelevare la somma finale dal buffer, 
	// 		solo quando sono state accumulati NUM_WORKER*NUM_REQ incrementi
	while (b->messaggi<NUM_REQ*NUM_WORKER)
	{
		pthread_cond_wait(&b->cv_collector,&b->mutex);
	}
	somma_finale=b->somma;
	
	

	return somma_finale;
}

void* collector(void* arg){
	int somma_finale;
	float media;

	// TODO recupera l'argomento
	Buffer *b=(Buffer*) arg;
	printf("[COLLECTOR] - start del thread...\n");
	somma_finale= preleva_somma_finale(b);

	// TODO preleva la somma chiamando preleva_somma_finale
	media = somma_finale/30;
	
	printf("[COLLECTOR] - somma finale: %d media: %f\n",somma_finale,media);
	
}

void *worker(void* arg){


	//TODO recupera i parametri
	parametri *par=(parametri*)arg;
	Misura m;
	printf("[WORKER] - start del thread... %d\n",par->id);

	//TODO ogni worker preleva NUM_REQ messaggi relativi ad un certo client id 
	//     e per ogni messaggio ricevuto aggiorna la somma chiamando aggiorna_somma
	for (int i = 0; i < NUM_REQ; i++)
	{
		printf("[WORKER] sto per ricevere misura dal sensore %d \n",par->id);

		msgrcv(coda,&m,sizeof(Misura)-sizeof(long),par->id,0);
		
	    printf("[WORKER] - Ho ricevuto la misura %d dal sensore %ld \n",m.misura,m.type);
		aggiorna_somma(par->b,m.misura);
	}

	
	pthread_exit(NULL);
	
	

}



int main(){

	int i,status;
	pid_t pid;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	key_t key;
	key = ftok(".",'k');
    
	coda = msgget(key,IPC_CREAT|0664);

	printf("[SERVER] - id coda %d\n",coda);


	//Inizializziamo strutture
	
	Buffer* b = malloc(sizeof(Buffer));

	init_buffer(b);
	parametri parametri[NUM_WORKER];
	
	//Avvio dei thread
	for(i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);
		
		// TODO creazione di 1 trhead collector e 3 thread worker (ai quali passare anche l'id)
		if (i==0)
		{
			pthread_create(&threads[0],&attr,collector,(void *) b);
		}else{
			parametri[i].b=b;
			parametri[i].id=i;
			pthread_create(&threads[i],&attr,worker,(void *) &parametri[i]);

		}
		
		
	}
	for(int j = 0; j < NUM_WORKER+1;j++){		
		pthread_join(threads[j],NULL);
	}

	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

    return 0;
}

