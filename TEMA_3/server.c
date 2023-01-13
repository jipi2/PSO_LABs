#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>

#define PORT_NUMBER 5050
#define IP_ADDR "127.0.0.1"

struct parameter
{
    int clientSocket;
};

pthread_key_t key;

void *thread_client_command(void *param)
{
    struct parameter *p = (struct parameter *)param;
    char *buffer = NULL;
    int dataSize;
    int clientSock;

    pthread_setspecific(key, &p->clientSocket);
    void *ptr = pthread_getspecific(key);
    
    clientSock = *(int*)ptr;
    
    dataSize = strlen("SERVER: Ne-am conectat\n");
    buffer = (char*)malloc(dataSize);
    strcpy(buffer,"SERVER: Ne-am conectat\n");

    send(clientSock, &dataSize, sizeof(int),0);
    send(clientSock, buffer,strlen(buffer),0);

    return NULL;
}

int main()
{
    int listenSocket, connectSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLength;
    struct parameter param;
    pthread_t threadId;

    pthread_key_create(&key, NULL);

    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDR);
    serverAddr.sin_port = PORT_NUMBER;

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, 5);

    while(1)
    {
        clientAddrLength = sizeof(clientAddr);
        connectSocket = accept(listenSocket, (struct sockaddr *) &clientAddr, &clientAddrLength);

        param.clientSocket = connectSocket;
        pthread_create(&threadId,NULL,&thread_client_command, &param);
        
    }

    return 0;
}