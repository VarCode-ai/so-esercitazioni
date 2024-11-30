#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	5

int a;

struct dati{
	char id;
	int *a;
};
void *fun(void* par)
{
   struct dati * d=(struct dati *) par;
   printf("Ciao sono il thread %c",d->id);
   for (int i = 0; i < 10; i++)
   {
	printf("%c\n",d->id);
	usleep(200000);
	(*d->a)++;
   }

   pthread_exit(NULL);
   
}

int main(int argc, char *argv[])
{
	int * a=malloc(sizeof(int));
	*a=0;//i thread condivino la memoria questa è la cosa importante
	//o creiamo variabili globali o le mettiamo nella memoria heap con malloc
	pthread_t th1,th2;
	struct dati d1;
	d1.id='a';
	d1.a=a;
	
	pthread_create(&th1,NULL,fun,(void *)&d1);

	struct dati d2;
	d2.id='b';
	d2.a=a;
	pthread_create(&th2,NULL,fun,(void *)&d2);

	pthread_join(th1,NULL);// questa struttura funziona perché ci stanno le join. le due struct stanno in aree di memoria non definite, lo stack senza join si potrebbe sovrascrivere e quindi il puntatore potrebbe non puntare alla struct. con le join invece evitiamo di sovrascrivere lo stack. in alternativa potremmo usare malloc.
	pthread_join(th2,NULL);

	printf("il valore di a è: %d\n",*a);
	free(a);
}

