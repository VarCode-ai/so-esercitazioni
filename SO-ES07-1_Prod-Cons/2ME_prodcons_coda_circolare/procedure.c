#include "procedure.h"
#include "semafori.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void produttore(prodcons *p, int sem_id, int index){
    Wait_Sem(sem_id,SPAZIO_DISP);
    Wait_Sem(sem_id,MUTEX_P);
    int num= rand() %30;
    sleep(index);
    p->buffer[p->testa] = num;
    printf("valore prodotto da PID: %d in posizione %d è ---> %d \n",getpid(),p->testa,num);
    p->testa=(p->testa +1)%DIM;
    Signal_Sem(sem_id,MUTEX_P);
    Signal_Sem(sem_id,NUM_MESS);
}

int consumatore(prodcons *p, int sem_id, int index){
    Wait_Sem(sem_id,NUM_MESS);
    Wait_Sem(sem_id,MUTEX_C);
    sleep(index);
    int valore_letto= p->buffer[p->coda];
    printf("valore CONSUMATO da PID: %d in posizione %d è ---> %d \n",getpid(),p->coda,valore_letto);
    p->coda=(p->coda +1)%DIM;
    Signal_Sem(sem_id,MUTEX_C);
    Signal_Sem(sem_id,SPAZIO_DISP);
    return valore_letto;
}