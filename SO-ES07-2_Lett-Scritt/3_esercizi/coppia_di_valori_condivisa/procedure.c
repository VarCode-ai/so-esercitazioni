#include "header.h"
#include "semafori.h"
#include "procedure.h"

void inizioLettura(Buffer *buf,int ds_sem){

    Wait_Sem(ds_sem,MUTEXL);
    buf->numlettori=buf->numlettori+1;
    if (buf->numlettori == 1)
    {
        //METTI IN ATTESA GLI SCRITTORI
        Wait_Sem(ds_sem,SYNCH);
    }
    Signal_Sem(ds_sem,MUTEXL);
}

void fineLettura(Buffer *buf,int ds_sem){
    Wait_Sem(ds_sem,MUTEXL);
    buf->numlettori=buf->numlettori-1;
    if (buf->numlettori==0)
    {
        Signal_Sem(ds_sem,SYNCH);
    }
    Signal_Sem(ds_sem,MUTEXL);
}

void inizioScrittura(int ds_sem){
    Wait_Sem(ds_sem,SYNCH);
}

void fineScrittura(int ds_sem){
    Signal_Sem(ds_sem,SYNCH);
}

