/* 2. Implementați o aplicație care paralelizează căutarea unui element într-un vector folosind procese și pipe-uri. 
Procesul inițial primește ca parametru numărul căutat, un fișier cu numere și numărul de procese. Fișierul cu numere va avea pe primul rând 
numărul de numere din fișier, apoi numerele efective.

Procesul principal va lansa în execuție procesele fiu. Fiecare proces fiu va căuta într-un interval din fișier și va transmite printr-un pipe 
către procesul părinte indexul sau indecșii unde a fost găsit numărul. Procesul părinte va centraliza și va afișa toți indecșii. */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int *vect = NULL;
int nr_wanted;

void print_part(int begin, int end)
{
    int i=0;
    for(i=begin;i<end;i++)
    {
        printf("%d\n", vect[i]);
    }

}

int *get_vector(int *nr_elemetns, char *path)
{
    int n=0;
    int *vect = NULL;

    FILE *f = fopen(path, "r");
    if(f==NULL)
    {
        return NULL;
    }

    fscanf(f, "%d", &n);

    vect = (int *)malloc(sizeof(int)*n);
    for(int i=0;i<n;i++)
        fscanf(f, "%d", &vect[i]);
    fclose(f);

    *nr_elemetns = n;
    return vect;
}

int **aloc_pipe(int n)
{
    int **pipe_fd = NULL;

    pipe_fd = (int **)malloc(sizeof(int*)*n);
    for(int i=0;i<n;i++)
        pipe_fd[i] = (int*)malloc(sizeof(int)*2);
    
    return pipe_fd;
}

void unloc_pipe(int **pipe_fd,int n)
{
    int i=0;
    for(i=0;i<n;i++)
    {
        free(pipe_fd[i]);
    }
    free(pipe_fd);
}

void search_in_vector(int begin, int end, int *fd)
{
    char buffer[256];
    char str_index[20];
    int size;
    int i=0;

    strcpy(buffer, "");

    for(i=begin;i<end;i++)
    {
        if(vect[i] == nr_wanted)
        {
            sprintf(str_index, "%d", i);
            strcat(buffer, str_index);
            strcat(buffer," ");
        }
    }

    size = strlen(buffer);
    if(size > 0)
        write(*fd, buffer, size);

    close(*fd);
}

int main(int argc, char *argv[])
{
    int nr_procese;
    int fd;
    pid_t *pid = NULL;
    char filename[256];
    int i;
    int nr_elements;
    int chunk;
    int **pipe_fd;
    char buffer[256];
    int bytesRed = 0;
    int flag = 0;

    if(argc < 4)
    {
        printf("Prea putini paramterii sefule\n");
        exit(-1);
    }

    nr_wanted = atoi(argv[1]);
    strcpy(filename, argv[2]);
    nr_procese = atoi(argv[3]);
    pid = (pid_t *)malloc(sizeof(pid_t)*nr_procese);

    vect = get_vector(&nr_elements,filename);
    chunk = nr_elements/nr_procese;

    pipe_fd = aloc_pipe(nr_procese);

    for(i=0;i<nr_procese;i++)
    {
        pipe(pipe_fd[i]);
        pid[i] = fork();
        if(pid[i] == 0)
        {
            close(pipe_fd[i][READ_END]);
            int begin, end;
            begin = i*chunk;

            if(i == nr_procese - 1)
                end = nr_elements;
            else
                end = begin+chunk;

            search_in_vector(begin, end, &pipe_fd[i][WRITE_END]);    

            exit(0);
        }
        close(pipe_fd[i][WRITE_END]);
    }

    for(i=0;i<nr_procese;i++)
    {
        waitpid(pid[i], NULL, 0);
        bytesRed = read(pipe_fd[i][READ_END], buffer, 256);
        if(bytesRed > 0)
        {
            buffer[bytesRed] = '\0';
            printf("%s", buffer);
            flag = 1;
        }
        close(pipe_fd[i][READ_END]);
    }

    free(vect);
 
    unloc_pipe(pipe_fd, nr_procese);

    if(flag == 0)
    {
        printf("Numarul nu s-a gasit\n");
    }

    printf("\n");

    return 0;
}