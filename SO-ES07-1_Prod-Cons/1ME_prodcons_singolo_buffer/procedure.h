#define NUM_PROD 2
#define NUM_CONS 2

#define MESSAGGIO_DISPONIBILE 0
#define SPAZIO_DISPONIBILE 1

void produttore (int *p, int sem);
void consumatore (int *p, int sem);