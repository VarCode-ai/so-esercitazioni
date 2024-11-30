#include "header.h"
#include <stdbool.h>
#include <stdio.h>
#include "sys/msg.h"
void mittente(int queue){
    Message mess;

    while (true)
    {
        printf("Tu: ");
        scanf("%s",mess.mess);
        printf("\n");
        mess.type=11;
        msgsnd(queue,(void *) &mess,sizeof(Message)-sizeof(long),0);
    }
    
}

void ricevente(int queue){
    while (true)
    {
        Message messaggio;
        msgrcv(queue,(void *) &messaggio,sizeof(Message)-sizeof(long),11,0);
        printf("Amico: %s\n",messaggio.mess);
    }
    
}