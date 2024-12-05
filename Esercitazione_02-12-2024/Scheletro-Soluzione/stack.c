#include "stack.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void StackInit(Stack * s, int dim) {
	s->dim=dim;
	s->top=-1;

	s->dati=(Elem*) malloc(sizeof(Elem)*dim);
	if (!s->dati) {
        perror("Errore di allocazione");
        exit(EXIT_FAILURE);
    }
	
	pthread_mutex_init(&s->mutex,NULL);
    pthread_cond_init(&s->ok_prod_cv,NULL);
    pthread_cond_init(&s->ok_cons_cv,NULL);



}


void StackRemove(Stack * s) {
	 if (s->dati) {
        free(s->dati);
    }
    pthread_mutex_destroy(&s->mutex);
    pthread_cond_destroy(&s->ok_prod_cv);
    pthread_cond_destroy(&s->ok_cons_cv);

}

void StackPush(Stack * s, Elem e) {
	//inizio produzione
	pthread_mutex_lock(&s->mutex);

	while (s->top>=s->dim-1)
	{
		pthread_cond_wait(&s->ok_prod_cv,&s->mutex);
	}

	//produzione
	s->top++;
    s->dati[s->top] = e;

	//fine produzione
	pthread_cond_signal(&(s->ok_cons_cv));
	pthread_mutex_unlock(&(s->mutex));
}


Elem StackPop(Stack * s) {
	pthread_mutex_lock(&s->mutex);
	int elemento;

	//inizio consumazione
	while (s->top<0)
	{
		pthread_cond_wait(&s->ok_cons_cv,&s->mutex);
	}//le condizione sono verificate con while perché il monitor all'interno dei thread è signal e continue

	//consumazione
	elemento = s->dati[s->top];
    s->top--;

	//fine consumazione
	
	pthread_cond_signal(&s->ok_prod_cv);
	pthread_mutex_unlock(&s->mutex);	

	return elemento;
}

int StackSize(Stack * s) {
	pthread_mutex_lock(&s->mutex);
	int size;

    size = s->top + 1;
    pthread_mutex_unlock(&s->mutex);
    return size;
	

}
