#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tiny.h"

#define BUFFSIZE (256)

char command[BUFFSIZE][BUFFSIZE];
int command_size;
char redirectFile[BUFFSIZE];
char command2[256][256];
int command2_size;

void runRedirectCommand()
{
    pid_t pid;
    char **command_copy;

    command_copy = calloc(command_size+1, sizeof(char*));
    for(int i=0;i<command_size;i++)
        command_copy[i] = command[i];

    int fd = open(redirectFile,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    pid = fork();
    if(pid == 0)
    {
        //child
        dup2(fd,1);
        execvp(command_copy[0],command_copy);
        exit(-1);
    }
    //parent
    close(fd);
    waitpid(pid,NULL,0);
    free(command_copy);
}

void runSimpleCommand()
{
    pid_t pid;
    char **command_copy;
    command_copy = calloc(command_size+1, sizeof(char*));
    for(int i=0;i<command_size;i++)
        command_copy[i] = command[i];

    pid = fork();
    if(pid == 0)
    {
        //child
        execvp(command_copy[0],command_copy);
        exit(-1);
    }
    //parent
    waitpid(pid,NULL,0);
    free(command_copy);
}

void runPipeCommand()
{
    int pipe_fd[2];
    pid_t pid1, pid2;
    pipe(pipe_fd);

    pid1 = fork();
    if(pid1 == 0)
    {
        close(STDOUT_FILENO);
        dup(pipe_fd[PIPE_WRITE]);
        close(pipe_fd[PIPE_READ]);
        close(pipe_fd[PIPE_WRITE]);

        char **command_copy;
        command_copy = calloc(command_size+1, sizeof(char*));
        for(int i=0;i<command_size;i++)
            command_copy[i] = command[i];
        execvp(command_copy[0],command_copy);
        
    }
    pid2 = fork();
    if(pid2 == 0)
    {
        dup2(pipe_fd[PIPE_READ],STDIN_FILENO);
        close(pipe_fd[PIPE_READ]);
        close (pipe_fd[PIPE_WRITE]);

        char **command_copy;
        command_copy = calloc(command2_size+1, sizeof(char*));
        for(int i=0;i<command2_size;i++)
            command_copy[i] = command2[i];
        execvp(command_copy[0],command_copy);
    }
    close(pipe_fd[PIPE_READ]);
    close(pipe_fd[PIPE_WRITE]);
    waitpid(pid1, NULL,0);
    waitpid(pid2, NULL,0);
}

int main()
{
    char readBuffer[BUFFSIZE];
    char *ret = NULL;
    int parse_code;

    do
    {
        printf("> ");
        ret = fgets(readBuffer,BUFFSIZE, stdin);
        if(ret == NULL)
            break;

        parse_code = parse(readBuffer);

        switch (parse_code)
        {
            case COMM_EXIT:
                goto final;
                break;
            
            case COMM_SIMPLE:
                runSimpleCommand();
                break;

            case NEW_LINE:
                break;

            case COMM_REDIRECT:
                runRedirectCommand();
                break;

            case COMM_PIPE:
                runPipeCommand();
                break;
        }
      
       command_size = 0;
    } while (parse_code != COMM_EXIT);
    
final:
    //cleanup
    return 0;
}