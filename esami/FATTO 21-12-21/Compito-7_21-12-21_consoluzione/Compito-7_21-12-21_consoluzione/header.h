#ifndef __HEADER
#define __HEADER

//Funzioni che ci servono
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define NUM_REQ 10

// TODO: completare il messaggio misura dai sensori
typedef struct{
	
	long type;
	
	int misura;
}Misura;

typedef struct{
	int somma;
	int counter;
	// TODO completare con le variabili per la sincronizzazione tra i thread
	pthread_mutex_t mutex;
	pthread_cond_t collector;
}Buffer;

#endif
