#ifndef __HEADER__
#define __HEADER__

#include <pthread.h>
#define NUM_THREADS 6
typedef int Elem;

typedef struct Stack {
	Elem * dati;	// puntatore ad un vettore dinamico di elementi
	int dim;		// dimensione dello Stack
	int top;
	/* mutex del monitor */
	pthread_mutex_t mutex;
	/* condition variables del monitor */
	pthread_cond_t ok_prod_cv;
	pthread_cond_t ok_cons_cv;



} Stack;

void StackInit(Stack * s, int dim);	// alloca dinamicamente "dim" elementi
void StackRemove(Stack * s);		// dealloca gli elementi
void StackPush(Stack * s, Elem e);	// inserimento nello Stack
Elem StackPop(Stack * s);			// estrazione dallo Stack
int StackSize(Stack * s);			// ritorna il numero attuale di elementi


#endif
