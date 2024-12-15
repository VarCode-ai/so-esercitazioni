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
	b->counter=0;

	pthread_mutex_init(&b->mutex,NULL);
	pthread_cond_init(&b->collector,NULL);
	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
	//TODO distruzione delle variabili de buffer
	pthread_cond_destroy(&b->collector);
	pthread_mutex_destroy(&b->mutex);
	printf("[SERVER] - Buffer distrutto...\n");
}


void aggiorna_somma(Buffer* b, int somma_parziale){
	// TODO aggiungere la somma parziale alla somma nel buffer,
	//		risvegliando il prelievo della somma finale quando necessario...
	pthread_mutex_lock(&b->mutex);
	b->somma+=somma_parziale;
	b->counter++;
	if (b->counter==NUM_REQ*NUM_WORKER)
	{
		pthread_cond_signal(&b->collector);
	}
	
	pthread_mutex_unlock(&b->mutex);

}

int preleva_somma_finale(Buffer* b){
	int somma_finale;
	//TODO prelevare la somma finale dal buffer, 
	// 		solo quando sono state accumulati NUM_WORKER*NUM_REQ incrementi
	pthread_mutex_lock(&b->mutex);
	
	while (b->counter<NUM_REQ*NUM_WORKER)
	{
		pthread_cond_wait(&b->collector,&b->mutex);
	}
	somma_finale=b->somma;
	
	pthread_mutex_unlock(&b->mutex);
	return somma_finale;
}

void* collector(void* arg){
	int somma_finale;
	float media;

	// TODO recupera l'argomento
	Buffer *b=(Buffer *) arg;
	printf("[COLLECTOR] - start del thread...\n");

	// TODO preleva la somma chiamando preleva_somma_finale
	somma_finale=preleva_somma_finale(b);
	media = somma_finale/b->counter;//TODO calcola la media sul totale delle misure ricevute
	
	printf("[COLLECTOR] - counter %d, somma finale: %d media: %f\n",b->counter,somma_finale,media);
	
}

void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	//TODO recupera i parametri
	parametri *p=(parametri*) arg;
	Buffer *b=p->b;
	int id=p->id;
	
	
	//TODO ogni worker preleva NUM_REQ messaggi relativi ad un certo client id 
	//     e per ogni messaggio ricevuto aggiorna la somma chiamando aggiorna_somma
	for (int i = 0; i < NUM_REQ; i++)
	{
		Misura m;
		msgrcv(coda,&m,sizeof(Misura)-sizeof(long),id,0);
		
		aggiorna_somma(b,m.misura);
		printf("[WORKER %d] - aggiornato somma di %d, somma attuale %d\n",i,m.misura,b->somma);
		
	}
	
	



}



int main(){

	int i,status;
	pid_t pid;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	key_t key;
	key = ftok(".",'c');
    
	coda = msgget(key,0);

	printf("[SERVER] - id coda %d\n",coda);


	//Inizializziamo strutture
	
	Buffer* b = malloc(sizeof(Buffer));

	init_buffer(b);
	
	
	//Avvio dei thread
	parametri par[NUM_WORKER];
	for(i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);

		// TODO creazione di 1 trhead collector e 3 thread worker (ai quali passare anche l'id)
		if (i==0)
		{
			pthread_create(&threads[i],&attr,collector,(void *) b);
		} else{
			par[i].id=i;
			par[i].b=b;
			pthread_create(&threads[i],&attr,worker,(void *)&par[i]);
		}
		
		
	}

	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

    return 0;
}

