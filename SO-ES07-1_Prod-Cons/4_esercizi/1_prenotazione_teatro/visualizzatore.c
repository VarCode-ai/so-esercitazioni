#include "sys/shm.h"
#include <sys/ipc.h>
#include "cliente.h"
#include "stdlib.h"
#include <stdio.h>
#include "unistd.h"
int main(){
    int keyshm=ftok("./shm",1);
    int ds_shm=shmget(keyshm,sizeof(prodcons),0);
    
    prodcons *p= shmat(ds_shm,NULL,0);
    
      while (1) {
        sleep(1);
        printf("/////////////////////////////// TEATRO %d /////////////////////////////////////////\n",p->disponibilità);
        for (int i = 0; i < POSTI; i++){
            printf("posto %d ----> stato: %d id_cliente: %d; ",i,p->posti[i].stato,p->posti[i].id_cliente);
        }
        printf("\n");
        if (p->disponibilità==0)
        {
            break;
        }

    }
    
    
    

    

}