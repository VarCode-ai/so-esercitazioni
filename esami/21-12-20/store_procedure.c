#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "store.h"

extern int ds_queue_request, ds_queue_response;

void Wait_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = -1;
    semop(ds_sem,&buf,1);
}

void Signal_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = 1;
    semop(ds_sem,&buf,1);
}

void init_magazzino(Magazzino *magazzino){
    magazzino->num_lettori=0;
    magazzino->num_scrittori=0;
    for (int i = 0; i < MAX_PRODOTTI; i++)
    {
        magazzino->scorta[i]=SCORTA_INIZIALE;
    }

    
    // TODO inizializzazione dei semafori , delle variabili necessarie alla sincronizzazione
    // e delle scorte di magazzino iniziali
   
}
void destroy_magazzino(Magazzino *magazzino){
    // TODO rimozione dei semafori
}

void lettore(Magazzino *magazzino) {
    int ret;
    Msg_Req m_req;
    Msg_Resp m_resp;
    int k;

    for (k=0; k<4; k++) {
        // TODO aggiungere codice per la ricezione comando
        

        // TODO Aggiungere codice per la lettura:
        InizioLettura(magazzino,k);
        int rand_id=rand()%3;
        ret = msgrcv(ds_queue_request,&m_req,sizeof(Msg_Req)-sizeof(long),rand()%3,0);
        FineLettura(magazzino,k);
        // InizioLettura
        // Lettura del livello di scorta del prodotto il cui id è indicato nella richiesta
        // FineLettura

        // TODO Costruire il messaggio di risposta m_resp
        m_resp.id_prodotto=rand_id;
        m_resp.pid=getppid();
        m_resp.scorta=ret;
        printf("[lettore] Ricevuta richiesta di lettura per prodotto %d con livello scorte %d\n",m_resp.id_prodotto,m_resp.scorta);

        // TODO aggiungere codice per l'invio del messaggio di risposta
    }
}

void scrittore(Magazzino *magazzino){
    int ret;
    Msg_Req m_req;
    int k;

    for (k = 0; k < 2; k++) {
        // TODO agiungere codice per la ricezione del comando
        

        // TODO aggiungere codice per la scrittura
        // InizioScrittura
        // Decremento del livello di scorta del prodotto indicato nella richiesta della quantità indicata nella richiesta
        // FineScrittura

        printf("[scrittore] Ricevuta richiesta di decremento di %d per prodotto %d\n",m_req.value, m_req.id_prodotto);
    }
}

void InizioLettura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di inizio lettura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    

    //il lettore può leggere sempre 
    Wait_Sem(magazzino->ds_sem_mutexl,id_prodotto);
    magazzino->num_lettori[id_prodotto]++;
    
    if (magazzino->num_lettori[id_prodotto] == 1)
    {
        Wait_Sem(magazzino->ds_sem_sync,id_prodotto);
    }

    Signal_Sem(magazzino->ds_sem_mutexl,id_prodotto);
    

}

void FineLettura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di fine lettura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    Wait_Sem(magazzino->ds_sem_mutexl,id_prodotto);
    magazzino->num_lettori[id_prodotto]--;
    if (magazzino->num_lettori[id_prodotto] == 0)
    {
        Signal_Sem(magazzino->ds_sem_sync,id_prodotto);
    }
    Signal_Sem(magazzino->ds_sem_mutexl,id_prodotto);



}

void InizioScrittura(Magazzino *magazzino, int id_prodotto){
    Wait_Sem(magazzino->ds_sem_sync,id_prodotto);
    //TODO aggiungere il codice di inizio scrittura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
}
void FineScrittura(Magazzino *magazzino, int id_prodotto){
    Signal_Sem(magazzino->ds_sem_sync,id_prodotto);
    //TODO aggiungere il codice di fine scrittura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
}
