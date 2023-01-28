#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void handler(int signum)
{
    if(signum == SIGINT)
    {
        printf("S-a primit semnalul\n");
    }
}

int main()
{
    struct sigaction sa;
    sigset_t set;

    memset(&sa, 0, sizeof(sa));
    sigemptyset(&set);
    sigaddset(&set, SIGINT); 

    sa.sa_mask = set;
    sa.sa_flags = SA_RESETHAND;
    sa.sa_handler = handler;

    sigaction(SIGINT, &sa, NULL);

    while(1){
        printf("12");
        sleep(2);
        sigpending(&set);
        if(sigismember(&set,SIGINT))
            break;
    } 

    printf("S-a terminat aplicatia\n");
    return 0;
}
