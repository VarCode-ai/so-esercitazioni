#include "pthread.h"
#define DIM_BUFFER 4
#define NUM_PRODUTTORI 5
#define NUM_PRODUZIONI 4
typedef struct{
    int testa;
    int coda;
    int uscita_timer;
    int count;
    int buf[DIM_BUFFER];

    pthread_mutex_t mutex;
    pthread_cond_t cv_prod;
    pthread_cond_t cv_cons;

} Monitor;

void inizializza_monitor(Monitor* monitor);
void distruggi_monitor(Monitor* monitor);

int produci(Monitor *monitor, int val);
int consuma(Monitor *m, int *val);

