/**
 * SO
 * Lab #4
 *
 * Task #5, Linux
 *
 * Avoid creating zombies using signals
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"

#define TIMEOUT		20

/*
 * configure signal handler
 */
static void ignore(int signo)
{
	if(signo == SIGCHLD)
	{
		wait(NULL);
	}
}

static void set_signals(void)
{
	struct sigaction sa;
	int rc;

	memset(&sa, 0, sizeof(sa));

	/* TODO - ignore SIGCHLD */

	sa.sa_flags = SIG_IGN;
	
	sa.sa_handler = ignore;

	rc = sigaction(SIGCHLD, &sa, NULL);
	DIE (rc == -1, "sigaction");

}

int main(void)
{
	pid_t pid;
	//pid_t pidParent;
	/* TODO - create child process without waiting */
	
	//pidParent = getpid();
	set_signals();
	pid  = fork();
	if(pid == 0)
	{
		/* const union sigval sig;
		sigqueue(pidParent,SIGCHLD, sig); */
		exit(0);
	}


	/* TODO - sleep */
	sleep(TIMEOUT);

	return 0;
}
