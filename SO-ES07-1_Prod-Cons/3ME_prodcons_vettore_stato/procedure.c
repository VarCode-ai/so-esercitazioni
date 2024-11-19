#include "procedure.h"
#include "semafori.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void produttore (prodcons *p,int semid,int i){
    Wait_Sem(semid, SPAZIO_DISPONIBILE);
    Wait_Sem(semid,MUTEXP);
    sleep(i);
    int index=0;
    for (int i = 0; i < DIM; i++)
    {
        if (p->stati[i]==LIBERO)
        {
            index=i;
            break;
        }
    }
    p->stati[index]=INUSO;
    Signal_Sem(semid,MUTEXP);
    int num=rand()%20;
    p->buffer[index]=num;
    p->stati[index]=PIENO;
    printf("valore PRODOTTO da PID: %d in posizione %d è ---> %d \n",getpid(),index,num);
    Signal_Sem(semid,MESSAGGIO_DISPONIBILE);

}

void consumatore(prodcons *p,int semid,int i){
    Wait_Sem(semid, MESSAGGIO_DISPONIBILE);
    Wait_Sem(semid,MUTEXC);
    sleep(i);

    int index=0;
    for (int i = 0; i < DIM; i++)
    {
        if (p->stati[i]==PIENO)
        {
            index=i;
            break;
        }
    }
    p->stati[index]=INUSO;
    Signal_Sem(semid,MUTEXC);

    int valore_letto=p->buffer[index];
    printf("valore CONSUMATO da PID: %d in posizione %d è ---> %d \n",getpid(),index,valore_letto);

    p->stati[index]=LIBERO;



    Signal_Sem(semid,SPAZIO_DISPONIBILE);
}