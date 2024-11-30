#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include "sys/shm.h"
#include "sys/sem.h"
#include "cliente.h"
#include "stdlib.h"
#include "sys/wait.h"
#include <sys/types.h>
#include <signal.h>
#include <time.h>

int main()
{
    key_t chiaveshm = ftok(".shm", 1);
    key_t chiavesem = ftok(".sem", 1);

    // shared memory
    int ds_shm = shmget(chiaveshm, sizeof(prodcons), IPC_CREAT | 0664);
    prodcons *p = shmat(ds_shm, NULL, 0);

    // semafori
    int ds_sem = semget(chiavesem, 1, IPC_CREAT | 0664);
    /*semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, 80);
    semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, 0);*/
    semctl(ds_sem, MUTEXP, SETVAL, 1);

    p->disponibilità = POSTI;

    for (int i = 0; i < POSTI; i++)
    {
        p->posti[i].stato = LIBERO;
        p->posti[i].id_cliente = 0;
    }

    int pidVisualizzatore = fork();
    if (pidVisualizzatore == 0)
    {
        execl("./visualizzatore", "visualizzatore", NULL);
        exit(1);
    }

    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        int pid = fork();
        srand(time(NULL)*pid);

        if (pid == 0)
        {
            cliente(p, ds_sem);
            exit(1);
        }
    }

    for (int i = 0; i < NUM_CLIENTI; i++)
    {
        int stato;
        int pid = wait(&stato);
        if (WIFEXITED(stato)) {
            int exit_code = WEXITSTATUS(stato);
            if (exit_code == 1) {
                //printf("Cliente terminato: prenotazione completata.\n");
            } else if (exit_code == 2) {
                printf("Cliente terminato: disponibilità terminata.\n");
            }
        }

       
    }
    printf("Tutti i clienti hanno terminato.\n");
    sleep(5);

    //kill(pidVisualizzatore,SIGTERM);
    waitpid(pidVisualizzatore, NULL, 0);

    shmctl(ds_shm, IPC_RMID, NULL);
    semctl(ds_sem, 0, IPC_RMID);
    return 0;
}