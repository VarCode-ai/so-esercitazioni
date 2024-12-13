#include <pthread.h>

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2
#define NUM_THREADS 5
#define NUMERO_BUFFER 3

typedef struct {
    int identificativo;
    int quota;
} Volo;

typedef struct {
    Volo vettore_voli[3];
    int vettore_stato[3];
    int numero_liberi;
    int numero_occupati;
    pthread_mutex_t mutex;
    pthread_cond_t cv_prod;

} GestioneVoli;

typedef struct{
    GestioneVoli *m;
    int id;
} parametri;

void InserisciVolo(GestioneVoli *g, int identificativo);
void RimuoviVolo(GestioneVoli *g, int identificativo);
void AggiornaVolo(GestioneVoli *g, int identificativo,int quota);
void init_monitor(GestioneVoli  *m);