#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

void work()
{
    while(1)
    {
        printf("Work\n");
        sleep(5);
    }
}

void handler()
{
    printf("S-a scris ceva\n");
    return ;
}

int main()
{

    pid_t pid;

    pid = fork();
    if(pid == 0)
    {
        work();
        exit(1);
    }
    else
    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = handler;
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGUSR1, &sa, NULL);

        while(1)
        {
            sleep(5);
        }
    }

    return 0;
}