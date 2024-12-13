#include "pthread.h"
#include "header.h"
#include <stdlib.h>
#include "sys/msg.h"
#include <unistd.h>

static int queue_req;
static int queue_rsp;



void consuma(Monitor *m, Msg_req *mess){
    pthread_mutex_lock(&m->mutex);
    while (m->count==0)
    {
        pthread_cond_wait(&m->ok_cons,&m->mutex);
    }

    m->count--;
    *mess = m->buffer[m->coda];
    m->coda=(m->coda+1)%DIM_BUFFER;
    pthread_cond_signal(&m->ok_prod);
    pthread_mutex_unlock(&m->mutex);        
}

void produci(Monitor *m, Msg_req mess){
    pthread_mutex_lock(&m->mutex);
    while (m->count==DIM_BUFFER)
    {
        pthread_cond_wait(&m->ok_prod,&m->mutex);
    }
    m->count++;
    m->buffer[m->testa]=mess;
    m->testa=(m->testa+1)%DIM_BUFFER;

    pthread_cond_signal(&m->ok_prod);
    pthread_mutex_unlock(&m->mutex); 
}
void * worker(void * arg){
    Monitor *m=(Monitor*)arg;

    Msg_req mess;
    consuma(m,&mess);

    int prodotto=mess.num1*mess.num2;
    Msg_rsp msg_rsp;
    msg_rsp.prodotto=prodotto;
    msg_rsp.type=mess.type;
    msgsnd(queue_rsp,&msg_rsp,sizeof(Msg_rsp)-sizeof(long),0);

}

void * manager(void * arg){
    Monitor *m=(Monitor*)arg;
    Msg_req msg_req;
    msgrcv(queue_req,&msg_req,sizeof(Msg_req)-sizeof(long),0,IPC_NOWAIT);
    sleep(1);
    produci(m,msg_req);

}

void init_monitor(Monitor *m){
    pthread_mutex_init(&m->mutex,NULL);
    pthread_cond_init(&m->ok_cons,NULL);
    pthread_cond_init(&m->ok_prod,NULL);

    m->testa=0;
    m->coda=0;
    m->count=0;
}
int main(){
    key_t key_req=ftok(".",'q');
    key_t key_rsp=ftok(".",'p');

    queue_req=msgget(key_req,0);
    queue_rsp=msgget(key_rsp,0);

    pthread_t thread_worker[NUM_WORKER];
    pthread_t thread_manager;
    Monitor *monitor=(Monitor *) malloc(sizeof(Monitor));
    init_monitor(monitor);

    for (int i = 0; i < NUM_WORKER; i++)
    {
        
        pthread_create(&thread_worker[i],NULL,worker,(void *)monitor);
    }
    
    pthread_create(&thread_manager,NULL,manager,(void *) monitor);

    for (int i = 0; i < NUM_WORKER; i++)
    {
        pthread_join(thread_worker[i],NULL);
    }
    
    pthread_join(thread_manager,NULL);

    


}