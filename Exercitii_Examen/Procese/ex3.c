/*
3. Process pool
Vom încerca să aplicăm conceptul de thread pool la procese. Vom avea un proces principal și N procese de tip worker lansate la pornirea aplicației.

Proces principal: Va crea N procese, un pipe cu nume și va intra într-o stare de waiting. 
Când va primi semnalul SIGUSR1, va trimite prin pipe zerouri cât să închidă toate procesele fiu, va șterge pipe-ul și își va încheia execuția.

Procese worker: Vor avea o buclă în care vor aștepta să primească instrucțiuni prin pipe-ul cu nume creat anterior. Procesele worker vor citi 
4 octeți din pipe. Dacă cei 4 octeți reprezintă un număr diferit de 0, se va afișa la terminal pid-ul procesului și numărul.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

int fd = -1;
char name[256];
int numberOfProc=0;
pid_t *pids;

pid_t *get_number_of_proc(int *nr_proc)
{
    int n;
    pid_t *pids = NULL;

    printf("Number of processes: ");
    scanf("%d", &n);

    if(n <= 0)
    {
        printf("The number > 0\n");
        exit(-1);

    }

    pids = (pid_t*)malloc(sizeof(pid_t)*n);
    
    *nr_proc = n;
    return pids;
}

void create_mkfifo()
{
    mkfifo(name, 0666);
}

void parent_write(int procNum)
{
    int n=0;
    int i=0;

    fd = open(name, O_WRONLY);

    if(fd < 0)
    {
        printf("The parent can not open the pipe\n");
        exit(-1);
    }

    do
    {
        if(n < 0)
            n=1;
        else
            n++;
        
        write(fd, &n, sizeof(int));
        sleep(1);
        

    } while (n != 0);

    close(fd);    

}

void worker_proc(int i)
{
    int n=0;
    int fd_c = open(name,O_RDONLY);
    if(fd_c < 0)
    {
        printf("The child with the pid: %d can not open the pipe\n", getpid());
        exit(-1);
    }

    do
    {
        read(fd_c, &n, sizeof(int));
        printf("Proces with the id: %d, and the number: %d recieved: %d\n", getpid(), i, n);

    } while (n != 0);

    close(fd_c);
}

static void handler(int signum)
{
    if(signum == SIGUSR1)
    {
        if(fd > 0)
            close(fd);
        
        fd = open(name, O_WRONLY);

        int n=0;
        int i=0;

        for(i=0;i<numberOfProc;i++)
        {
            write(fd, &n, sizeof(int));
            sleep(1);
        }

        close(fd);

        for(i=0;i<numberOfProc;i++)
        {
            waitpid(pids[i], NULL, 0);
        }
        exit(0);
    }
}

void set_signals()
{
//-------------------------------sigaction-------------------------------------------------------//
    int rt = 0;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    rt = sigaction(SIGUSR1, &sa, NULL);
    if(rt < 0)
    {
        printf("Nu e bine la semnal\n");

        exit(-1);
    }
//-------------------------------sigaction-------------------------------------------------------//
}

int main()
{
    int i=0;

    pids = get_number_of_proc(&numberOfProc);

    strcpy(name, "fifo_smeck");
    create_mkfifo(name);
    

    for(i=0;i<numberOfProc;i++)
    {
        pids[i] = fork();
        if(pids[i] == 0)
        {
            worker_proc(i);
            exit(0);
        }
    }


    set_signals();
    parent_write(numberOfProc);

    for(i=0;i<numberOfProc;i++)
    {
        waitpid(pids[i],NULL,0);
    }

    free(pids);
    

    return 0;
}