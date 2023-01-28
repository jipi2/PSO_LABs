/**
 * SO, 2016
 * Lab #4
 *
 * Task #4, Linux
 *
 * Simulate nohup command
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

#define NOHUP_OUT_FILE		"nohup.out"

/* configure handlers */

static void ignore_hup(int signo)
{
	switch (signo)
	{
	case SIGHUP:
	printf("SIGHUP ignorat cu succes\n");
	break;
	
	case SIGINT:
	printf("SIGINT ignorat cu succes\n");
	break;

	default:
		break;
	}
}

static void set_signals(void)
{
	struct sigaction sa;
	int rc;

	memset(&sa, 0, sizeof(sa));

	/* TODO - ignore SIGHUP */
	sa.sa_flags = SA_SIGINFO;
	sa.sa_handler = ignore_hup;
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
}

/* execute a new program */
static void exec_func(int argc, char **argv)
{
	int rc;
	int i;
	char **exec_args;

	/* ignore SIGHUP */
	set_signals();

	/* TODO - if stdout is a terminal
	 * redirect output to NOHUP_OUT_FILE
	 */
	
	int fd = open(NOHUP_OUT_FILE, O_CREAT | O_APPEND | O_RDWR, 0644);
	if(fd < 0)
	{
		printf("Eroare la dechiderea fisierului \n");
		exit(-1);
	}
	if(isatty(STDOUT_FILENO))
	{
		dup2(fd,STDOUT_FILENO);
	}

	/* exec a new process */
	exec_args = malloc(argc * sizeof(*exec_args));
	DIE(exec_args == NULL, "malloc");

	for (i = 0; i < argc-1; i++)
		exec_args[i] = argv[i+1];
	exec_args[argc-1] = NULL;

	execvp(exec_args[0], exec_args);
	DIE(1, "execvp");
}

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s command_and_arguments\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	exec_func(argc, argv);

	return 0;
}
