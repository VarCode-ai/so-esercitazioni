
#define DIM 4
#define NUM_CONS 8
#define NUM_PROD 8

#define MUTEXP 0
#define MUTEXC 1
#define MESSAGGIO_DISPONIBILE 2
#define SPAZIO_DISPONIBILE 3

#define LIBERO 0
#define PIENO 2
#define INUSO 3
typedef struct{
    int buffer[DIM];
    int stati[DIM];
} prodcons;

void produttore (prodcons *p,int semid,int index);
void consumatore(prodcons *p,int semid,int index);