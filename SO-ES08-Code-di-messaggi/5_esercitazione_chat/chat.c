#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"
#include <stdio.h>
struct message {
    long msg_type;
    char text[MAX_TEXT];
};

int main(int argc, char *argv[]){
    if (argc != 3)
    {
        printf("errore devi mettere key1 e key2");
        exit(0);
    }

    

    //chiavi
    printf("%s",argv[1]);
    printf("%s",argv[2]);

    int key_queue_1=ftok(".",*argv[1]);
    int key_queue_2=ftok(".",*argv[2]);


    //code
    int queue1=msgget(key_queue_1,IPC_CREAT|0664);
    int queue2=msgget(key_queue_2,IPC_CREAT|0664);
    printf("queue1: %d\n",key_queue_1);
    printf("queue2: %d\n",key_queue_2);

    int pid_mittente=fork();
    if (pid_mittente==0)
    {
        mittente(queue1);
    }
    
    int pid_ricevente=fork();
    if (pid_ricevente==0)
    {
        ricevente(queue2);
    }

    wait(NULL);
    wait(NULL);


    
}