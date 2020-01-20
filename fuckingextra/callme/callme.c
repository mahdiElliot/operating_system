
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

int sigusr1(); 
void sigusr2(); 

int pid; 

void main() 
{ 

	/* get child process */
	if ((pid = fork()) < 0) { 
		perror("fork"); 
		exit(1); 
	} 

	if (pid == 0) { /* child */
		signal(SIGUSR1, sigusr1); 
        
		while(1);/* loop for ever */
	} 
	else /* parent */
	{ /* pid hold id of child */
		printf("\nPARENT: sending SIGUSR1\n\n"); 
		kill(pid, SIGUSR1);

        int c = signal(SIGUSR2,sigusr2);
        while(1);
	} 
} 

int sigusr1() 
{ 
	signal(SIGUSR1, sigusr1); /* reset signal */
	printf("CHILD: I have received a SIGUSR1\n"); 
    printf("\nCHILD: sending SIGUSR2\n\n"); 
	kill(pid, SIGUSR2);
    return 1;
} 

void sigusr2() 

{ 
	signal(SIGUSR2,sigusr2); /* reset signal */
	printf("PARENT: I have received a SIGUSR2\n");
    exit(0); 
} 

