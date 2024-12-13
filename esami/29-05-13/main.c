#include "header.h"
#include "stdio.h"
#include "sys/msg.h"
#include <unistd.h>
int main(){

    //code di messaggi
    key_t key_req=ftok(".",'q');
    key_t key_rsp=ftok(".",'p');

    int queue_req=msgget(key_req,IPC_CREAT|0664);
    int queue_rsp=msgget(key_rsp,IPC_CREAT|0664);
    
    //processi
    int pid;
    for (int i = 0; i < NUM_CLIENT; i++)
    {
        pid=fork();
        
        if (pid==0)
        {
            if (i==0)
            {
                printf("[MAIN] ESEGUO SERVER");
                execl("./server","server",NULL);
            }
            else{
                printf("[MAIN] ESEGUO CLIENT");
                execl("./client","client",NULL);
            }   
        }
        
    }
    

}