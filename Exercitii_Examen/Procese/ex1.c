/* 1. Scrieți o aplicație care primește ca parametru un 
număr(n) și lansează în execuție n procese. Aceste procese 
vor afișa la terminal indexul lor(al câtălea a fost creat). */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_func(int i)
{
    printf("Sunt un proces copil cu indexul: %d\n", i);
}

int main()
{
    pid_t *pid;
    int i=0;
    int n;

    printf("Introduce un numar n care va reprezenta numarul de procese: ");
    scanf("%d", &n);

    pid = (pid_t*)malloc(sizeof(pid_t)*n);

    for(i=0;i<n;i++)
    {
        pid[i] = fork();
        if(pid[i] == 0)
        {
            child_func(i+1);
            break;
        }
    }

   for(i=0;i<n;i++)
    waitpid(pid[i], NULL, 0);
}