#include "semafori.h"
#include "procedure.h"
#include  <stdio.h>


void produttore(int *p,int sem_id){

    Wait_Sem(sem_id,SPAZIO_DISPONIBILE);
    *p=*p+2;
    Signal_Sem(sem_id,MESSAGGIO_DISPONIBILE);


}

void consumatore(int *p,int sem_id){
    Wait_Sem(sem_id,MESSAGGIO_DISPONIBILE);
    printf("valore letto: %d \n",*p);
    Signal_Sem(sem_id,SPAZIO_DISPONIBILE);
}