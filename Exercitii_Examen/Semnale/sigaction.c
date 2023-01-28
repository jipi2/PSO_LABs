#include <signal.h>
#include <unistd.h>
 
#include <stdio.h>
#include <string.h>
 
 
void usr2_handler(int signum) {
	if (signum == SIGUSR2) {
		printf("SIGUSR2 detected\n");
	}
}
 
int main() 
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
 
	printf("Process %d started.\n", getpid());
 
	sa.sa_flags = SA_RESETHAND;
	sa.sa_handler = usr2_handler;
	sigaction(SIGUSR2, &sa, NULL);
 
	while (1) {
		sleep(1);
	}
 
	return 0;
}