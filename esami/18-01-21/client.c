#include <sys/msg.h>
#include "header.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){

    key_t msg_key=ftok(".",'m');
    int ds_msg=msgget(msg_key,0);
    printf("eseguo cliente con id %d\n",atoi(argv[1]));

    if (argc < 2 ) {
		printf("ERRORE: il client deve ricevere il proprio ID\n");
		return -1;
	}
    	srand(time(NULL));

    
    for (int i = 0; i < NUM_MISURE; i++)
    {
        msg mes;
        mes.value=rand()%101;
        mes.type=atol(argv[1]);
        printf("invio messaggio tipo %ld con valore %d\n",mes.type,mes.value);
        msgsnd(ds_msg,&mes,sizeof(msg)-sizeof(long),0);
    }
    
	printf("[Client %d] - Fine processo\n",getpid());

    return 0;
}