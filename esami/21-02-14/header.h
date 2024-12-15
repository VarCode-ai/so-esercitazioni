#include "pthread.h"
#define NUM_CAPITRENO 4
#define NUM_LETTORI 10
typedef struct  { 
 // riporta la stazione in cui si trova il treno (intero tra 0 e 10) 
 int stazione; 
 
// aggiungere qui variabili per la sincronizzazione 
 int num_lettori;
 int num_scrittori;
  int num_lettori_wait;
 int num_scrittori_wait;
 pthread_cond_t cv_scrittori;
 pthread_cond_t cv_lettori;

 pthread_mutex_t mutex;
}monitor_treno; 
 
void inizializza( monitor_treno * m); 
void rimuovi( monitor_treno * m); 
int leggi_stazione( monitor_treno * m); 
void scrivi_stazione( monitor_treno * m, int stazione);