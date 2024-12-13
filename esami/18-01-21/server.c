#include <sys/msg.h>
#include "header.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_WORKER 3

static pthread_t threads[NUM_WORKER+1];
static int ds_msg;
typedef struct{
    Buffer *buf;
    int id;
}parametri;

void init_buffer(Buffer *buf){
    buf->somma=0;
    buf->num_elementi=0;
    pthread_mutex_init(&buf->mutex,NULL);
    pthread_cond_init(&buf->cv_collector,NULL);
}

void aggiorna(Buffer *buf, int value){
    pthread_mutex_lock(&buf->mutex);
    buf->somma+=value;
    buf->num_elementi+=1;
    if (buf->num_elementi==NUM_SENSORI*NUM_MISURE)
    {
        pthread_cond_signal(&buf->cv_collector);
    }
    pthread_mutex_unlock(&buf->mutex);
}
void* worker(void * arg){
    parametri *par=(parametri *) arg;
    
    for (int i = 0; i < NUM_MISURE; i++)
    {
        msg mes;    
        msgrcv(ds_msg,&mes,sizeof(msg)-sizeof(long),par->id,0);
        aggiorna(par->buf,mes.value);
    }
    pthread_exit(0);

    
}


void* collector(void * arg){
    Buffer *b = (Buffer *) arg;
    pthread_mutex_lock(&b->mutex);
    pthread_cond_wait(&b->cv_collector,&b->mutex);
        float media=b->somma/30;
        printf("la mmedia della somma di %d elementi è %f con somma %d",b->num_elementi,media,b->somma);
    pthread_mutex_unlock(&b->mutex);
    pthread_exit(0);

}

int main(){

    //MESSAGGI
    int key_msg=ftok(".",'m');
    ds_msg=msgget(key_msg,0);

    //BUFFER
    Buffer *buf=malloc(sizeof(Buffer));
    init_buffer(buf);
    parametri param[NUM_WORKER];

    //THREAD
    for(int i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);

		if(i == 0){
			pthread_create(&threads[i],NULL,collector,(void *) buf);
		}else{
			param[i].id= i;
			param[i].buf = buf;
			pthread_create(&threads[i],NULL,worker,(void *) &param[i]);
		}
	}

	printf("[MASTER] - bye bye...\n");
	pthread_exit(NULL);
   
    
    
    


    
    return 0;
}