#include "pthread.h"
#define NUM_CLIENT 3
#define NUM_WORKER 2
#define DIM_BUFFER 3

typedef struct {
    long type;
    int num1;
    int num2;
} Msg_req;

typedef struct {
    long type;
    int prodotto;
} Msg_rsp;

typedef struct {
    Msg_req buffer[DIM_BUFFER];
    int testa;
    int coda;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t ok_prod;
    pthread_cond_t ok_cons;
    
} Monitor;