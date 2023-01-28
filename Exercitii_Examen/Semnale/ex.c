#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(int signum)
{
    if(signum == SIGINT)
        printf("TZEAPA");
}

int main()
{
    struct sigaction sa;
    int count = 0;

    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);

    while(1)
    {
        
    }

    return 0;
}