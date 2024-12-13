#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    int buffer[MAX_VALUES];
    int testa;
    int coda;
    int contatore;
    // TODO aggiungere le variabili per la gestione circolare
    // e per la sincronizzazione dei thread
    pthread_mutex_t mutex;
    pthread_cond_t cv_spazio;
    pthread_cond_t cv_messaggio;

} data_str;

typedef struct {
    int somma;
    int num_valori_sommati;

    pthread_mutex_t mutex;
    pthread_cond_t cv_max_elem;
    // TODO aggiungere le variabili per la sincronizzazione dei thread
} somma_valori;

//struttura per passare i parametri somma_valori e data_str ai thread
typedef struct {
    somma_valori* sum;
    data_str* buf;
} parametri;


// le code
int queue_req;
int queue_res;

int consuma(data_str* buf) {
    pthread_mutex_lock(&buf->mutex);
    if (buf->contatore==0)
    {
        pthread_cond_wait(&buf->cv_messaggio,&buf->mutex);
    }
    
    int value;
    //TODO consumatore, con gestione coda circolare
    buf->testa=(buf->testa+1)%MAX_VALUES;
    buf->contatore--;
    value=buf->buffer[buf->testa];
    pthread_cond_signal(&buf->cv_spazio);
    pthread_mutex_unlock(&buf->mutex);

    return value;
}

void produci(data_str* buf, int new_value) {
    //TODO produttore, con gestione coda circolare
    pthread_mutex_lock(&buf->mutex);
    if (buf->contatore==MAX_VALUES)
    {
        pthread_cond_wait(&buf->cv_spazio,&buf->mutex);
    }
    buf->coda=(buf->coda+1)%MAX_VALUES;
    buf->buffer[buf->coda] =new_value;
    buf->contatore++;
    pthread_cond_signal(&buf->cv_messaggio);
    pthread_mutex_unlock(&buf->mutex);
   
}


void * stampa_somma(void* par) {
    
    somma_valori * s = (somma_valori*)par;
    pthread_mutex_lock(&s->mutex);
    //TODO attesa che siano stati sommati NUM_CONS valori utilizzando una condition
    // e stampa della somma una volta verificata la condizione
    pthread_cond_wait(&s->cv_max_elem,&s->mutex);
    printf("[STAMPA]: somma di %d elementi %d\n",s->num_valori_sommati,s->somma);
    pthread_mutex_unlock(&s->mutex);

    pthread_exit(NULL);
}

void * produttore(void* par) {
    data_str * buf = (data_str *) par;
    int new_value;
    int i;
    for (i = 0; i < NUM_UPDATES*MAX_VALUES; i++) {
        new_value = rand()%10+1;
        printf("PRODUTTORE: inserimento nuovo dato: %d\n",new_value);
        produci(buf,new_value);
        sleep(rand()%3+1);
    }
    pthread_exit(NULL);
}


void * consumatore(void* par) {
    parametri * p = (parametri *) par;
    data_str* buf = p->buf;
    somma_valori* sum = p->sum;
    int i;
    for (i = 0; i < NUM_CONS; i++) {
        req msg;
        //TODO ricezione messaggio di richiesta
        int result = msgrcv(queue_req,(void*)&msg,sizeof(req)-sizeof(long),1,0);
        if(result < 0){ 
	        perror("CONSUMATORE_SERV: errore nella ricezione di un messaggio\n");
            break; 
            return(NULL);
        }
        printf("CONSUMATORE_SERV: ricevuta richiesta di consumo di tipo %ld\n",msg.type);

        res risp;
        //TODO preparazione messaggio di risposta
        //prelevando il valore dalla funzione "consuma"
        risp.value = consuma(buf);
        risp.type = 1;
        printf("CONSUMATORE_SERV: invio valore al consumatore client %d\n",risp.value);
        //TODO invio risposta
        msgsnd(queue_res,(void*)&risp,sizeof(res)-sizeof(long),0);

        pthread_mutex_lock(&sum->mutex);
        //TODO aggiornamento in mutua esclusione della somma in 'sum' con il nuovo valore appena consumato
        //TODO e notifica della condizione quando sommati NUM_CONS elementi
        sum->num_valori_sommati++;
        sum->somma+=risp.value;
        if (sum->num_valori_sommati == NUM_CONS)
        {
            pthread_cond_signal(&sum->cv_max_elem);
        }
        pthread_mutex_unlock(&sum->mutex);

    }

    pthread_exit(NULL);
}

int main() {

    //TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok(".",'q');
	key_t msg_res_key = ftok(".",'s');

    queue_req = msgget(msg_req_key,0);
	queue_res = msgget(msg_res_key,0);
    if(queue_req < 0) {
		perror("Msgget fallita");
		exit(1);
	}
    if(queue_res < 0) {
		perror("Msgget fallita");
		exit(1);
	}

    pthread_t prod, cons, sum;

    srand(time(NULL));

    somma_valori* s =(somma_valori*) malloc(sizeof(somma_valori));

    // TODO inizializzazione campi di s
    s->num_valori_sommati=0;
    s->somma=0;
    pthread_mutex_init(&s->mutex,NULL);
    pthread_cond_init(&s->cv_max_elem,NULL);
    
    data_str* shared_buf = (data_str*) malloc(sizeof(data_str));
    // TODO inizializzazione campi di shared_buf
    shared_buf->testa=-1;
    shared_buf->coda=-1;
    for (int i = 0; i < MAX_VALUES; i++)
    {
        shared_buf->buffer[i]=0;
    }
    pthread_cond_init(&shared_buf->cv_messaggio,NULL);
    pthread_cond_init(&shared_buf->cv_spazio,NULL);
    pthread_mutex_init(&shared_buf->mutex,NULL);



    parametri * par = (parametri*) malloc(sizeof(parametri));
    // TODO inizializzazione campi di par
    par->buf=shared_buf;
    par->sum=s;

    //TODO creazione dei tre thread prod cons e sum
    /* nota: al thread produttore basta passare solo il buffer 'shared_buf'
             al thread stampa basta passare solo la somma 's'
             al thread consumatore bisogna passare entrambi tramite 'par'
    */
    pthread_create(&prod,NULL,produttore,(void *)shared_buf);
    pthread_create(&cons,NULL,consumatore,(void *)par);
    pthread_create(&sum,NULL,stampa_somma,(void *)s);
    

    //TODO join 
    pthread_join(prod,NULL);
    pthread_join(cons,NULL);
    pthread_join(sum,NULL);

    //TODO rimozione code e strutture dati
    msgctl(queue_req,IPC_RMID,NULL);
    msgctl(queue_res,IPC_RMID,NULL);
    pthread_mutex_destroy(&shared_buf->mutex);
    pthread_mutex_destroy(&s->mutex);
    pthread_cond_destroy(&shared_buf->cv_messaggio);
    pthread_cond_destroy(&shared_buf->cv_spazio);
    pthread_cond_destroy(&s->cv_max_elem);

    free(shared_buf);
    free(s);
    free(par);
    return 0;
}
