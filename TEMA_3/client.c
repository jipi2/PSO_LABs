#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT_NUMBER 5050
#define IP_ADDR "127.0.0.1"

int main()
{
    int socketDescriptor;
    struct sockaddr_in serverAddr;
    char *buffer = NULL;
    int dataSize = 0;
    int rc;

    socketDescriptor = socket(AF_INET, SOCK_STREAM,0);
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = PORT_NUMBER;
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDR);

    buffer = (char*)malloc(sizeof(char)*100);
    
    rc = connect(socketDescriptor, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(rc < 0)
    {
        printf("ERROR CONNECTION\n");
        exit(-1);
    }


    recv(socketDescriptor, &dataSize, sizeof(int), 0);
    recv(socketDescriptor, buffer, dataSize, 0);

    printf("%s",buffer);
    

    close(socketDescriptor);

    return 0;
}