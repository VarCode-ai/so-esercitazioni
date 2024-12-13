#include <sys/msg.h>
#include "header.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){

    //messaggi
    key_t key_msg=ftok(".",'m');
    int ds_msg=msgget(key_msg,IPC_CREAT|0664);

    int i;
    for (i = 0; i < NUM_SENSORI; i++)
    {
        sleep(1);
        int pid=fork();
        if (pid==0)
        {
        
            char id_client=i+'1';
            printf("creo cliente con id %d\n",i+1);
            execl("./client","client",&id_client,NULL);
            exit(0);
        }
        
    }

    int pidServer=fork();
    if (pidServer==0)
    {
        printf("eseguo server");
        execl("./server","server",NULL);
        exit(0);
  
    }

    for (int j = 0; j< NUM_SENSORI+1; j++)
    {
        wait(NULL);
    }
    
    
    return 0;
}