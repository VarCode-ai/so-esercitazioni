#include "processi.h"
#include "semafori.h"

#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int inizializza_semafori()
{
    int sem_id = semget(IPC_PRIVATE,1,IPC_CREAT|0664);/* TBD: usare semget() per allocare un vettore,
                  *      con un singolo semaforo "mutex" */

    if (sem_id < 0)
    {
        perror("Impossibile creare il semaforo per la mutua esclusione");
        exit(1);
    }

    /* Valore iniziale: 1 (mutua esclusione) */
    semctl(sem_id,0,SETVAL,1);
    printf("SEMAFORO %d INIZIALIZZATO AD 1\n", sem_id);

    /* TBD: inizializzare il mutex */

    return sem_id;
}

void figlio(int *vettore,
            int *buffer,
            int sem_id,
            int elemento_iniziale,
            int qta_elementi)
{

    /* TBD: aggiungere dentro questa funzione delle chiamate a
     *      Wait_Sem() e Signal_Sem() per creare una sezione critica
     *      e realizzare la mutua esclusione */

    int minimo = INT_MAX;

    for (int i = elemento_iniziale; i < elemento_iniziale + qta_elementi; i++)
    {
        if (vettore[i] < minimo)
        {

            minimo = vettore[i];
        }
    }

    printf("Figlio: Il minimo locale è %d\n", minimo);

    if (minimo < *buffer)
    {
        Wait_Sem(sem_id,0);
        *buffer = minimo;
        Signal_Sem(sem_id,0);
    }
}


void padre(int *buffer, int sem_id)
{


    for (int i = 0; i < NUM_PROCESSI; i++)
    {
        wait(NULL);
    }
    /* Attesa terminazione processi figli */

    /* TBD: Utilizzare wait() per attendere la terminazione dei 10 figli */

    

    /* Risultato finale */

    printf("Padre: Il valore minimo assoluto è: %d\n", *buffer);
}
