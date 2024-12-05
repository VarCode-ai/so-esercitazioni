#ifndef _MONITOR_H_
#define _MONITOR_H_

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2

#define DIM 5

#include <pthread.h>

typedef struct {

    char vettore[DIM];
    int stato[DIM];

    int num_occupati;
    int num_liberi;

    pthread_mutex_t mutex;
    pthread_cond_t cv_produttori;
    pthread_cond_t cv_consumatori;
    pthread_cond_t cv_attesa;

} MonitorIO;

void init_monitor(MonitorIO * m);
void end_monitor(MonitorIO * m);
void produzione(MonitorIO * m, char c);
char consumazione(MonitorIO * m);
int attesa(MonitorIO * m);

#endif