#include <pthread.h>
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void InserisciVolo(GestioneVoli *g, int identificativo){
    pthread_mutex_lock(&g->mutex);
    int i;
    while (g->numero_liberi==0)
    {
        pthread_cond_wait(&g->cv_prod,&g->mutex);
    }

    for (i = 0; i < 3; i++)
    {
        if (g->vettore_stato[i]==LIBERO)
        {
            break;
        } 
    }

    g->vettore_stato[i]=IN_USO;
    g->numero_liberi--;
    pthread_mutex_unlock(&g->mutex);
    sleep(1);
    printf("[PRODUTTORE]: produzione volo con id: %d",identificativo);

    g->vettore_voli[i].identificativo=identificativo;
    g->vettore_voli[i].quota=0;

    pthread_mutex_lock(&g->mutex);
    g->numero_occupati++;
    g->vettore_stato[i]=OCCUPATO;
    pthread_mutex_unlock(&g->mutex);
    printf("Uscita monitor - produzione\n");

}
void RimuoviVolo(GestioneVoli *g, int identificativo){
    pthread_mutex_lock(&g->mutex);
    int index =-1;
    for (int  i = 0; i < 3; i++)
    {
        if (g->vettore_voli[i].identificativo==identificativo)
        {
            index=i;
            break;
        }
        
    }
    if (index==-1)
    {
        printf("Errore: Volo %d non trovato\n", identificativo);

		pthread_mutex_unlock( &(g->mutex) );

		return;
    }
    
    
        g->numero_occupati--;
        g->vettore_stato[index]=IN_USO;
        pthread_mutex_unlock(&g->mutex);

        printf("[CONSUMATORE]: consumazione volo con id: %d in posizione: %d\n",identificativo,index);
        sleep(1);
        pthread_mutex_lock(&g->mutex);
        g->numero_liberi++;
        g->vettore_stato[index]=LIBERO;
        pthread_cond_signal(&g->cv_prod);

    
    
    pthread_mutex_unlock(&g->mutex);
    
    printf("Uscita monitor - consumazione\n");


}
void AggiornaVolo(GestioneVoli *g, int identificativo,int quota){
    pthread_mutex_lock(&g->mutex);
    int index=-1;
    for (int i = 0; i < 3; i++)
    {
        if (g->vettore_voli[i].identificativo=identificativo)
        {
            index=i;
            break;
        }
    }

    if (index==-1)
    {
        printf("Errore: Volo %d non trovato\n", identificativo);

		pthread_mutex_unlock( &(g->mutex) );

		return;
    }
    
    
        g->vettore_stato[index]=IN_USO;
        pthread_mutex_unlock( &(g->mutex) );
	    printf("[debug] aggiornamento in [%d] del volo %d\n\n", index, identificativo);

        sleep(rand()%3+1);
        printf("[AGGIORNATORE]: aggiorno volo con id: %d in posizione: %d\n",identificativo,index);
        g->vettore_voli[index].quota=quota;
        pthread_mutex_lock( &(g->mutex) );

        g->vettore_stato[index]=OCCUPATO;
    
    pthread_mutex_unlock(&g->mutex);

}
void init_monitor(GestioneVoli  *m){
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->cv_prod,NULL);

    m->numero_liberi=NUMERO_BUFFER;
    m->numero_occupati=0;
    for (int i = 0; i < 3; i++)
    {
        m->vettore_stato[i]=LIBERO;
    }
    
}

int ricerca_volo(GestioneVoli * g, int identificativo) {

	int id_buffer = -1;

	int i=0;


	while( i<NUMERO_BUFFER && id_buffer==-1 ) {

		if(g->vettore_stato[i] == OCCUPATO &&
		   g->vettore_voli[i].identificativo == identificativo) {

			id_buffer = i;
		}

		i++;
	}


	return id_buffer;
}