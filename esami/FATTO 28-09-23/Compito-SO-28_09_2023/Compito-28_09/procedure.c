#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "header.h"


int Wait_Sem (int id_sem, int numsem) {
       int err;
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;

       err = semop(id_sem,&sem_buf,1);   //semaforo rosso

       if(err<0) {
         perror("Errore WAIT");
       }

       return err;
}


int Signal_Sem (int id_sem, int numsem) {
       int err;
       struct sembuf sem_buf;

       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;

       err = semop(id_sem,&sem_buf,1);   //semaforo verde

       if(err<0) {
         perror("Errore SIGNAL");
       }

       return err;
}

void produci(struct Coda * c, int ds_sem, int num_prenotazioni) {
    Wait_Sem(ds_sem,SPAZIO_DISPONIBILE);
    Wait_Sem(ds_sem,MUTEX_P);
    c->buffer[c->testa]=num_prenotazioni;
    c->testa= (c->testa+1) %DIM_BUFFER;
    Signal_Sem(ds_sem,MUTEX_P);
      Signal_Sem(ds_sem,MESSAGGIO_DISPONIBILE);

    //TODO completare il codice del produttore
}

int consuma(struct Coda * c, int ds_sem) {
    int consumato;
    Wait_Sem(ds_sem,MESSAGGIO_DISPONIBILE);
    Wait_Sem(ds_sem,MUTEX_C);
    consumato=c->buffer[c->coda];
    c->coda= (c->coda+1) % DIM_BUFFER;
    Signal_Sem(ds_sem,SPAZIO_DISPONIBILE);
    Signal_Sem(ds_sem,MUTEX_C);
   	//TODO completare il codice del consumatore
    
    return consumato;
}

