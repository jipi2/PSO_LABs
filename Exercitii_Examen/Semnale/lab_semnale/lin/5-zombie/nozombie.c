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

#include "utils.h"

#define TIMEOUT		20

/*
 * configure signal handler
 */

static void ignore(signum)
{
	if(signum == SIGCHLD)
		printf("Ignore mate\n");
}

static void set_signals(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	



	/* TODO - ignore SIGCHLD */
	sa.sa_handler = ignore;
	sigaction(SIGCHLD, &sa, NULL);

}

int main(void)
{
	pid_t pid;

	set_signals();


	/* TODO - create child process without waiting */
	pid = fork();
	if(pid == 0)
	{
		exit(0);
	}
	else
	{
		sleep(TIMEOUT);
	}

	/* TODO - sleep */

	return 0;
}
