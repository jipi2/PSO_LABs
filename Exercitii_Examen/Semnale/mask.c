#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void ignore(int signum)
{
    if(signum == SIGINT)
    {
        printf("ajsndjasnknsdknfnsdakfnlsdnflndslfnsdanf\n");
    }
}

int main()
{
    struct sigaction sa;

    int count = 0;

    memset(&sa, 0, sizeof(sa));

    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = ignore;

    sigaction(SIGINT, &sa, NULL);

    sleep(20);

    return 0;
}