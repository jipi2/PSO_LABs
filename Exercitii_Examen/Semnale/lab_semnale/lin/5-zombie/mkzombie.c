/**
 * SO
 * Lab #4
 *
 * Task #5, Linux
 *
 * Creating zombies
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define TIMEOUT		20

int main(void)
{
	pid_t pid;

	/* TODO - create child process without waiting */
	pid = fork();
	if(pid == 0)
	{
		exit(0);
	}

	/* TODO - sleep */
	sleep(TIMEOUT);

	return 0;
}
