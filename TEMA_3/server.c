#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <signal.h>

#define PORT_NUMBER 5050
#define IP_ADDR "127.0.0.1"

#define BUFF_SIZE 4096
#define OK 1
#define N_OK -1

#define LS "ls"
#define BYE "bye"

struct parameter
{
    int clientSocket;
};

pthread_key_t key;
pthread_t threadIds[10];
int thread_count = 0;

char *ls_func()
{
    int sizeOfName = 0;
    DIR *dir;
    struct dirent *ent;
    int count = 0;
    char *files = NULL;
    char filename[1024];

    files = (char *)malloc(sizeof(char) * BUFF_SIZE);
    strcpy(files, "");

    strcat(files, ";");

    dir = opendir(".");
    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG)
            {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, "server") != 0 &&
                    strcmp(ent->d_name, "server.c") != 0 && strcmp(ent->d_name, "client") != 0 && strcmp(ent->d_name, "client.c") != 0 && strcmp(ent->d_name, "Makefile") != 0)
                {
                    strcpy(filename, ent->d_name);
                    strcat(files, filename);
                    strcat(files, ";");
                }
            }
        }
        closedir(dir);
    }
    return files;
}

void format_str(char *str) // foloseste ca separator ";", practic in loc de ";" pun "\0"
{
    int n = strlen(str);

    for (int i = 0; i < n; i++)
    {
        if (str[i] == ';')
            str[i] = '\0';
    }
}

void ls_send(char *command, int *sock)
{
    int clientSocket;
    char *files = ls_func();
    int len = 0;
    int rt = 0;
    char status[2];
    char number[8];
    char answ[BUFF_SIZE];

    printf("%s\n", command);

    strcpy(answ,"");

    sprintf(status,"%d",OK);
    strcat(answ, status);
    strcat(answ, ";");

    len = strlen(files);
    sprintf(number,"%d",len);
    strcat(answ,number);


    len = strlen(answ);

    strcat(answ, files);

    len = strlen(answ);
    format_str(answ);
    
    clientSocket = *sock;

    rt = write(clientSocket, answ, len);
    if(rt < 0)
    {
        printf("EROARE\n");
        return;
    }
    
    free(files);
    //free(answ);
}

void *thread_client_command(void *param)
{
    // -----------------------------------------------------------------------------------------------------------------
    // begining

    printf("%d\n", thread_count);
    struct parameter *p = (struct parameter *)param;
    char *buffer = NULL;
    int dataSize;
    int clientSock;
    char command[BUFF_SIZE];


    pthread_setspecific(key, &p->clientSocket);
    void *ptr = pthread_getspecific(key);

    clientSock = *(int *)ptr;
    //------------------------------------------------------------------------------------------------------------------
    // connection is oke

    buffer = (char *)malloc(sizeof(char) * BUFF_SIZE);
    strcpy(buffer, "SERVER: Ne-am conectat\n");
    write(clientSock, buffer, strlen(buffer));

    free(buffer);

    //------------------------------------------------------------------------------------------------------------------

    do
    {
        dataSize = read(clientSock, command, BUFF_SIZE);
        command[dataSize] = '\0';

        if (strcmp(command, LS) == 0)
        {
            ls_send(command, &clientSock);
        }
        else if(strcmp(command, BYE)==0)
        {
            break;
        }


    } while (strcpy(command, "bye") != 0);

    close(clientSock);

    //mutex
    thread_count--;
    //mutex

    return NULL;
}

void exit_handle(int signum)
{
    if(signum == SIGTERM || signum == SIGINT)
    {
        printf("Graceful Termination\n");

        for(int i=0;i<thread_count;i++)
        {
            pthread_join(threadIds[i],NULL);
        }
    }
}
int main()
{
    int listenSocket, connectSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLength;
    struct parameter param;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_RESETHAND;
    sa.sa_handler = exit_handle;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    pthread_key_create(&key, NULL);

    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDR);
    serverAddr.sin_port = PORT_NUMBER;

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, 5);

    while (1)
    {
        clientAddrLength = sizeof(clientAddr);
        connectSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrLength);

        param.clientSocket = connectSocket;
        pthread_create(&threadIds[thread_count++], NULL, &thread_client_command, &param);
    }

    return 0;
}