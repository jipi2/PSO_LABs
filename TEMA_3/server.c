#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT_NUMBER 5051
#define IP_ADDR "127.0.0.1"

#define BUFF_SIZE 4096

void ls_func(char *buffer, int *sock)
{
    int size = read(*sock, buffer, BUFF_SIZE);
    /* int size;

    recv(*sock, &size, sizeof(int), 0);
    recv(*sock, buffer, size, 0); */

    for (int i = 0; i < size - 1; i++)
    {
        if (buffer[i] == '\0')
            buffer[i] = '\n';
    }

    printf("%s\n", buffer);
}

void get_func(char *command, int *sock)
{
    int status = 0;
    int size = 0;
    char *buffer = NULL;
    int n = 0;
    int bytesRed = 0;

    read(*sock, &status, sizeof(int));

    printf("Status = %d\n", status);

    if(status == 0)
        return;
    
    read(*sock, &size, sizeof(int));
    printf("Size = %d\n", size);

    buffer = (char*)malloc(sizeof(char)*size);
    strcpy(buffer,"");

    while(bytesRed < size)
    {
        n = read(*sock, buffer+bytesRed, size);
        buffer[n+bytesRed] = '\0';
        bytesRed += n;
    }

    printf("%s\n", buffer);

}

void get_delete(char *command, int *sock)
{
    int status;
    read(*sock, &status, sizeof(int));

    printf("Status = %d\n", status);
}

void put_func(char *command, int *sock)
{
    int status = 0;

    read(*sock, &status, sizeof(int));

    printf("Status = %d\n", status);
}

void update_func(int *sock)
{
    int status = 0;

    read(*sock, &status, sizeof(int));

    printf("Status = %d\n", status);
}

int main()
{
    int socketDescriptor;
    struct sockaddr_in serverAddr;
    char *buffer = NULL;
    char *command = NULL;
    int dataSize = 0;
    int rc;

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = PORT_NUMBER;
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDR);

    buffer = (char *)malloc(sizeof(char) * 4096);
    command = (char *)malloc(sizeof(char) * 4096);

    rc = connect(socketDescriptor, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (rc < 0)
    {
        printf("ERROR CONNECTION\n");
        exit(-1);
    }

    dataSize = read(socketDescriptor, buffer, 4096);

    /* recv(socketDescriptor, &dataSize, sizeof(int), 0);
    recv(socketDescriptor, buffer, dataSize, 0); */

    buffer[dataSize] = '\0';
    printf("%s", buffer);

 
    fgets(command, 4096, stdin);
    command[strlen(command) - 1] = '\0';
    write(socketDescriptor, command, strlen(command));

    /* dataSize = strlen(command);
    send(socketDescriptor, &dataSize, sizeof(int),0);
    send(socketDescriptor, command, dataSize, 0); */

    if (strcmp(command, "list") == 0)
        ls_func(buffer, &socketDescriptor);
    else if(strstr(command, "get"))
    {
        get_func(command, &socketDescriptor);    //get;<numar_octeti_nume_fisier>;<nume_fisier>
    }
    else if (strstr(command, "put;"))
    {
        put_func(command, &socketDescriptor);
    }
    else if (strstr(command, "delete"))
    {
        get_delete(command, &socketDescriptor);
    }
    else if (strstr(command, "update"))
    {
        update_func(&socketDescriptor);
    }

    printf("\n-----------------------------------------------------------\n");

   

    close(socketDescriptor);

    free(buffer);
    free(command);

    return 0;
}
