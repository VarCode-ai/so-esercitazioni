#include "cliente.h"
#include "semafori.h"
#include <unistd.h>
#include "stdlib.h"
#include <stdio.h>

void cliente(prodcons *p,int ds_sem){
    Wait_Sem(ds_sem,MUTEXP);
    int num = rand() % 4 +1;
    sleep(rand()%5);
    printf("%d",p->disponibilità);
    if (p->disponibilità<num)
    {
        Signal_Sem(ds_sem,MUTEXP);
        exit(2);
    }
    printf("Cliente %d: tento di prenotare %d posti.\n", getpid(), num);

    int prenotati = 0;
    for (int i = 0; i < POSTI && prenotati < num; i++) {
        if (p->posti[i].stato == LIBERO) {
            p->posti[i].stato = IN_AGGIORNAMNTO; // in_aggiornamento
            p->posti[i].id_cliente = getpid();
            prenotati++;
        }
    }
    p->disponibilità-=num;
    Signal_Sem(ds_sem,MUTEXP);
    

    sleep(1);
    for (int i = 0; i < POSTI; i++) {
        if (p->posti[i].stato == IN_AGGIORNAMNTO && p->posti[i].id_cliente == getpid()) {
            p->posti[i].stato = OCCUPATO; // occupato
        }
    }
    

    /*for (int j = 0; j < num; j++)
    {
        int index;
        for (int i = 0; i < POSTI; i++)
        {
            if (p->posti[i].stato==LIBERO)
            {
                index=i;
                break;
            }
        }
        p->posti[index].stato=IN_AGGIORNAMNTO;

        sleep(1);
        p->posti[index].stato=OCCUPATO;
        p->posti[index].id_cliente=getpid();
        Signal_Sem(ds_sem,MUTEXP);

        printf("cliente %d ha occupato posto %d\n",getpid(),index);

    }*/
    
    


    
}