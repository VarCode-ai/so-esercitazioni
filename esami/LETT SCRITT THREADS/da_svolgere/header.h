#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>


//////////// LETT-SCRITT STARVATION ENTRAMBI THREAD ////////////

struct monitor {

	int molo;
	int id_nave;

	int num_lettori;
	int num_scrittori;

	int num_lettori_wait;
	int num_scrittori_wait;

	pthread_cond_t ok_lett_cv;
	pthread_cond_t ok_scritt_cv;
	pthread_mutex_t mutex;
};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void scrittura(struct monitor * m, int molo);
int lettura(struct monitor * m);



#endif
