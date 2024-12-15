#ifndef __HEADER
#define __HEADER

#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEX_P 2
#define MUTEX_C 3

#define DIM_BUFFER 5

#define NUM_PRODUTTORI 3
#define NUM_CONSUMATORI 3

#define NUM_REQ 10

struct Coda {
    int buffer[DIM_BUFFER];
    int testa;
    int coda;
};

void produci(struct Coda *, int id_sem, int num_prenotazioni);
int consuma(struct Coda *, int id_sem);

// num prenotazioni condivisa lato server
typedef struct{
	pthread_mutex_t mutex;
	int num_prenotazioni;
}Count;

#endif
