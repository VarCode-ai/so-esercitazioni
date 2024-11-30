
#define DIM 4
#define NUM_PRODUTTORI 15
#define NUM_CONSUMATORI 15

#define SPAZIO_DISP 0
#define NUM_MESS 1
#define MUTEX_P 2
#define MUTEX_C 3
typedef struct {
    int buffer[DIM];
    int testa;
    int coda;
} prodcons;

void produttore (prodcons *p,int sem_id,int index);
int consumatore (prodcons *p,int sem_id,int index);