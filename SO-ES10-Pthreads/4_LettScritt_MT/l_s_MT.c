/*****PROBLEMA DEI LETTORI-SCRITTORI: soluzione mediante Threads*****/
/*Il programma sincronizza lettori e scrittori nell'accesso ad una variabile condivisa utilizzando i Pthreads. */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "header.h"


#define NUM_THREADS     10


int main(){

     pthread_attr_t attr;
     pthread_t threads[NUM_THREADS];


     //creazione di una istanza di struttura monitor
     struct LettScritt * ls = malloc(sizeof(struct LettScritt));//la struttura da condividere con i thread o la mettiamo in ambiente globale o in area heap(meglio in area heap)


     //importante non mettere le variabili in shared memory con i pthread
     //le variabili in shared memory servonom per condividere valori tra vari processi (creati con fork o exec) ma i thread sono tutti appartenenti allo stesso processo 
     //quindi nons erve mettere in shared memory


     //inizializzazione mutex e condition
     pthread_mutex_init(&ls->mutex,NULL);
     pthread_cond_init(&ls->ok_lett_cv,NULL);
     pthread_cond_init(&ls->ok_scritt_cv,NULL);

     //inizializzazione delle variabili di stato
     ls->num_lettori = 0;
     ls->num_scrittori = 0;
     ls->num_lettori_wait = 0;
     ls->num_scrittori_wait = 0;



     //impostazione dei thread come joinable
     pthread_attr_init(&attr);
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

     //generazione di scrittori e lettori
     int k;
     for (k=0;k<NUM_THREADS; k++) {

	if (k%2)  {
	     printf("Sono il thread Lettore (%d)\n",k);
	     pthread_create(&threads[k], &attr, Lettore, (void *) ls);//attr potrebbe pure essere NULL
	   } else {
	     sleep(1);
             printf("Sono il thread Scrittore (%d)\n",k);
	     pthread_create(&threads[k], &attr, Scrittore, (void *) ls);
	    }
	}

     for (k=0; k<NUM_THREADS;k++){
       pthread_join(threads[k],NULL);
       printf ("Thread n.ro %d terminato\n ",k);
     }

     /*deallocazione risorse*/	
     pthread_attr_destroy(&attr);
     pthread_mutex_destroy(&ls->mutex);
     pthread_cond_destroy(&ls->ok_lett_cv);
     pthread_cond_destroy(&ls->ok_scritt_cv);

     free(ls);//libera memoria

     pthread_exit(0);
}


