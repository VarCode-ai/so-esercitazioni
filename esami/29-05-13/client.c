#include "header.h"
#include "stdio.h"
#include "sys/msg.h"
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
    key_t key_req=ftok(".",'q');
    key_t key_rsp=ftok(".",'p');

    int queue_req=msgget(key_req,0);
    int queue_rsp=msgget(key_rsp,0);

    for (int i = 0; i < 5; i++)
    {
        Msg_req msg_req;
        msg_req.num1=rand() %11;
        msg_req.num2=rand() %11;
        msg_req.type=getpid();
        msgsnd(queue_req,&msg_req,sizeof(Msg_req)-sizeof(long),0);

        Msg_rsp msg_rsp;
        msgrcv(queue_rsp,&msg_rsp,sizeof(Msg_rsp)-sizeof(long),getpid(),0);
    }
    
}