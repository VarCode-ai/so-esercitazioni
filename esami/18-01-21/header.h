#include <pthread.h>
#define NUM_SENSORI 3
#define NUM_MISURE 10
typedef struct{
    long type;
    int value;
} msg;

typedef struct{
    int somma;
    int num_elementi;
    pthread_mutex_t mutex;
    pthread_cond_t cv_collector;

} Buffer;