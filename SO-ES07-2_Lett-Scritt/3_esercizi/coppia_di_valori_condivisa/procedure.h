#include "header.h"

#define MUTEXL 0
#define SYNCH 1

#define NUM_LETTORI 2


typedef struct{
    int val1;
    int val2;
    int numscrittori;
    int numlettori;

} Buffer;

void inizioLettura(Buffer *buf,int ds_sem);
void fineLettura(Buffer *buf,int ds_sem);
void inizioScrittura(int ds_sem);
void fineScrittura(int ds_sem);
