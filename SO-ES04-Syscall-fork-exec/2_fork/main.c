#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

	int N = 3, i, pid;
	for (i = 0; i < N; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			printf("Sono il FIGLIO con PID: %d\n", getpid());
			// exit(0);
			i = 3;
		}
	}

	// Attende i figli per evitare processi zombie
	for (int i = 0; i < N; i++)
	{
		wait(NULL);
	}
	printf("Istruzione successiva \n");
	return 0;
}
