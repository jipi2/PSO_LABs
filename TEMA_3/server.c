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
#include <sys/sendfile.h>
#include <fcntl.h>

#define PORT_NUMBER 5051
#define IP_ADDR "127.0.0.1"

#define BUFF_SIZE 4096
#define OK 1
#define N_OK -1
#define MAX_FILES_NUMBER 100

#define LIST "list"
#define BYE "bye"
#define GET "get"
#define DELETE "delete"
#define PUT "put"
#define UPDATE "update"

struct parameter
{
    int clientSocket;
};

pthread_key_t key;
pthread_t threadIds[10];
int thread_count = 0;
pthread_mutex_t counterMutex;
pthread_mutex_t modifyStructMutex;

struct FilesStruct
{
    char **files;
    int *counter;
    pthread_mutex_t *counter_lock;
    pthread_mutex_t *write_lock;
    int size;
};

struct FilesStruct filesInUse;

int isFileInUse(char *filename)
{
    for (int i = 0; i < filesInUse.size; i++)
        if (strcmp(filename, filesInUse.files[i]) == 0)
            return 1;
    return 0;
}

void add_file_in_FilesInUse(char *filename)
{
    filesInUse.files[filesInUse.size] = strdup(filename);
    pthread_mutex_init(&filesInUse.counter_lock[filesInUse.size], NULL);
    pthread_mutex_init(&filesInUse.write_lock[filesInUse.size], NULL);
    filesInUse.counter[filesInUse.size] = 0;
    filesInUse.size++;
}

int get_Index_of_file_in_use(char *filename)
{
     for (int i = 0; i < filesInUse.size; i++)
        if (strcmp(filename, filesInUse.files[i]) == 0)
            return i;
    
    pthread_mutex_lock(&modifyStructMutex);
    add_file_in_FilesInUse(filename);
    pthread_mutex_unlock(&modifyStructMutex);

    return (filesInUse.size-1);
}

void init_FilesStructure()
{
    filesInUse.files = (char**)malloc(sizeof(char*)*MAX_FILES_NUMBER);
    filesInUse.counter = (int*)malloc(sizeof(int)*MAX_FILES_NUMBER);
    filesInUse.counter_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)*MAX_FILES_NUMBER);
    filesInUse.write_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)*MAX_FILES_NUMBER);
    filesInUse.size = 0;
}

void destroy_FilesInUseStructure()
{
    int i=0;
    for(i=0;i<filesInUse.size;i++)
        free(filesInUse.files[i]);
    free(filesInUse.files);
    free(filesInUse.counter_lock);
    free(filesInUse.write_lock);
    free(filesInUse.counter);
}

void lock_for_writing(pthread_mutex_t *writeMutex)
{
    pthread_mutex_lock(writeMutex);
}

void unlock_for_writing(pthread_mutex_t *writeMutex)
{
    pthread_mutex_unlock(writeMutex);
}

void lock_for_reading(pthread_mutex_t *counterMutex, pthread_mutex_t *writeMutex, int *counter)
{
    pthread_mutex_lock(counterMutex);
    (*counter)++;
    if ((*counter) == 1)
    {
        pthread_mutex_lock(writeMutex);
    }
    pthread_mutex_unlock(counterMutex);
}

void unlock_for_reading(pthread_mutex_t *counterMutex, pthread_mutex_t *writeMutex, int *counter)
{
    pthread_mutex_lock(counterMutex);
    *(counter)--;
    if ((*counter) == 0)
    {
        pthread_mutex_unlock(writeMutex);
    }
    pthread_mutex_unlock(counterMutex);
}

void init_mutexes()
{
    pthread_mutex_init(&counterMutex, NULL);
    pthread_mutex_init(&modifyStructMutex, NULL);
}

void destroy_mutexes()
{
    pthread_mutex_destroy(&counterMutex);
    pthread_mutex_destroy(&modifyStructMutex);
}

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

    strcpy(answ, "");

    sprintf(status, "%d", OK);
    strcat(answ, status);
    strcat(answ, ";");

    len = strlen(files);
    sprintf(number, "%d", len);
    strcat(answ, number);

    len = strlen(answ);

    strcat(answ, files);

    len = strlen(answ);
    format_str(answ);

    clientSocket = *sock;

    rt = write(clientSocket, answ, len);
    if (rt < 0)
    {
        printf("EROARE\n");
        return;
    }

    free(files);
    // free(answ);
}

void send_file(char *filename, int *client)
{
    int size = 0;
    int status = 0;
    int fd = 0;
    off_t bytesWrote = 0;
    int n = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        status = 0;
        write(*client, &status, sizeof(int));
        return;
    }
    else
    {
        status = 1;
        write(*client, &status, sizeof(int));
    }

    size = lseek(fd, 0, SEEK_END);
    write(*client, &size, sizeof(int));

    while (bytesWrote < size)
    {
        n = sendfile(*client, fd, &bytesWrote, size);
        bytesWrote += n;
    }
}

char *getFileName(char *command)
{
    char *p;
    char *fileName = NULL;
    char delim[] = ";:\n ";
    int numBytes = 0;

    p = strtok(command, delim);
    p = strtok(NULL, delim);
    numBytes = atoi(p);

    fileName = (char *)malloc(sizeof(char) * numBytes);

    p = strtok(NULL, delim);

    if (strlen(p) > numBytes)
        return NULL;

    strcpy(fileName, p);
    return fileName;
}

void get_funct(char *command, int *client)
{
    // comanda clientului este separata prin ";"
    char *fileName;
    int index = 0;

    printf("%s\n", command);

    fileName = getFileName(command);

    index = get_Index_of_file_in_use(fileName); 
    lock_for_reading(&filesInUse.counter_lock[index], &filesInUse.write_lock[index], &filesInUse.counter[index]);
    send_file(fileName, client);
    unlock_for_reading(&filesInUse.counter_lock[index], &filesInUse.write_lock[index], &filesInUse.counter[index]);

    free(fileName);
}

int delete_file(char *fileName)
{
    int status = 0;
    DIR *dir;
    struct dirent *entry;

    if (fileName == NULL)
    {
        return status;
    }

    if (strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0 || strcmp(fileName, "server") == 0 ||
        strcmp(fileName, "server.c") == 0 || strcmp(fileName, "client") == 0 || strcmp(fileName, "client.c") == 0 || strcmp(fileName, "Makefile") == 0)
    {
        return 0;
    }

    dir = opendir(".");
    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                if (strcmp(entry->d_name, fileName) == 0)
                {
                    if (unlink(fileName) == 0)
                    {
                        status = 1;
                        return status;
                    }
                    else
                    {
                        return status;
                    }
                }
            }
        }
        closedir(dir);
    }
    else
    {
        return status;
    }

    return status;
}

void deleteFunct(char *command, int *client)
{
    char *fileName;
    int status = 0;

    printf("%s\n", command);
    fileName = getFileName(command);

    status = delete_file(fileName);
    write(*client, &status, sizeof(int));

    free(fileName);
}

int make_file(char *fileName, char *content, int sizeContent)
{
    int fd;
    int bytesWrote = 0;
    int n = 0;

    fd = open(fileName, O_WRONLY | O_CREAT, 0644);
    if (fd < 0)
    {
        return 0;
    }

    while (bytesWrote < sizeContent)
    {
        n = write(fd, content + bytesWrote, sizeContent - bytesWrote);
        bytesWrote += n;
        if (n < 0)
        {
            close(fd);
            return 0;
        }
    }

    close(fd);

    return 1;
}

void putFunct(char *command, int *client)
{
    char *p;
    char *fileName;
    int sizeNameF = 0;
    char delim[] = ";";
    int sizeContent = 0;
    char *content = NULL;
    int status = 0;

    p = strtok(command, delim);
    p = strtok(NULL, delim);
    sizeNameF = atoi(p);

    p = strtok(NULL, delim);
    if (strlen(p) < sizeNameF)
    {
        write(*client, &status, sizeof(int));
        return;
    }

    fileName = (char *)malloc(sizeof(char) * sizeNameF);
    strcpy(fileName, p);

    p = strtok(NULL, delim);
    sizeContent = atoi(p);

    p = strtok(NULL, delim);
    content = (char *)malloc(sizeof(char) * sizeContent);

    strncpy(content, p, sizeContent);

    content[sizeContent] = '\0';

    printf("%s\n", fileName);
    printf("%d\n", sizeContent);
    printf("%s\n", content);

    status = make_file(fileName, content, sizeContent);

    write(*client, &status, sizeof(int));

    free(fileName);
    free(content);
}

char *getOpcode(char *command)
{
    char *aux = NULL;
    int i = 0;
    int size = 0;

    for (i = 0; i < strlen(command); i++)
    {
        size++;
        if (command[i] == ';')
        {
            break;
        }
    }

    aux = (char *)malloc(sizeof(char) * size);
    strncpy(aux, command, size - 1);
    aux[size - 1] = '\0';

    return aux;
}

int updateFile(char *fileName, int startByte, char *content, int sizeContent)
{
    int fd;
    int initFileSize = 0;
    int firstPartSize = 0;
    int lastPartSize = 0;
    int allContentSize = 0;
    int n = 0;
    int bytesWrote = 0;
    char *allContent = NULL;
    char *firstPart = NULL;
    char *lastPart = NULL;
    int index = 0;

    fd = open(fileName, O_RDWR);
    if (fd < 0)
    {
        return 0;
    }

    index = get_Index_of_file_in_use(fileName);
    lock_for_writing(&filesInUse.write_lock[index]);

    initFileSize = lseek(fd, 0, SEEK_END);
    if (startByte >= initFileSize)
    {
        while (bytesWrote < sizeContent)
        {
            n = write(fd, content + bytesWrote, sizeContent - bytesWrote);
            bytesWrote += n;
            if (n < 0)
            {
                close(fd);
                return 0;
            }
        }
        lock_for_writing(&filesInUse.write_lock[index]);
        return 1;
    }
    else
    {
        lseek(fd, 0, SEEK_SET);
        firstPart = (char *)malloc(sizeof(char) * startByte);
        read(fd, firstPart, startByte);
        firstPart[startByte] = '\0';

        int n1 = lseek(fd, startByte, SEEK_SET);
        lseek(fd, 0, SEEK_SET);
        int n2 = lseek(fd, 0, SEEK_END);

        lastPartSize = n2 - n1;

        lseek(fd, startByte, SEEK_SET);
        lastPart = (char *)malloc(sizeof(char) * lastPartSize);
        read(fd, lastPart, lastPartSize);
        lastPart[lastPartSize] = '\0';

        allContentSize = n2 + sizeContent;

        allContent = (char *)malloc(sizeof(char) * allContentSize);

        strcpy(allContent, firstPart);
        strcat(allContent, content);
        strcat(allContent, lastPart);

        bytesWrote = 0;
        n = 0;

        lseek(fd, 0, SEEK_SET);
        while (bytesWrote < allContentSize)
        {
            n = write(fd, allContent + bytesWrote, allContentSize - bytesWrote);
            bytesWrote += n;
            if (n < 0)
            {
                close(fd);
                return 0;
            }
        }
        close(fd);
    }
    unlock_for_writing(&filesInUse.write_lock[index]);
    return 1;
}

void updateFunct(char *command, int *client)
{
    char *p;
    char *fileName;
    int sizeNameF = 0;
    char delim[] = ";";
    int sizeContent = 0;
    char *content = NULL;
    int status = 0;
    int startByte = 0;

    p = strtok(command, delim);
    p = strtok(NULL, delim);
    sizeNameF = atoi(p);

    p = strtok(NULL, delim);
    if (strlen(p) > sizeNameF)
    {
        write(*client, &status, sizeof(int));

        return;
    }

    fileName = (char *)malloc(sizeof(char) * sizeNameF);
    strcpy(fileName, p);

    p = strtok(NULL, delim);
    startByte = atoi(p);

    p = strtok(NULL, delim);
    sizeContent = atoi(p);
    content = (char *)malloc(sizeof(char) * sizeContent);
    strcpy(content, "");

    p = strtok(NULL, delim);
    strncpy(content, p, sizeContent);
    content[sizeContent] = '\0';

    printf("%s\n%d\n%s\n%d\n", fileName, startByte, content, sizeContent);

    status = updateFile(fileName, startByte, content, sizeContent);

    write(*client, &status, sizeof(status));

    free(fileName);
    free(content);
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
    char *opcode = NULL;

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

    dataSize = read(clientSock, command, BUFF_SIZE);
    if (dataSize < 0)
    {
        close(clientSock);
        free(opcode);

        pthread_mutex_lock(&counterMutex);
        thread_count--;
        pthread_mutex_unlock(&counterMutex);

        return NULL;
    }
    command[dataSize] = '\0';

    if (strcmp(command, LIST) == 0)
    {
        ls_send(command, &clientSock);
    }

    opcode = getOpcode(command);

    if (strcmp(opcode, GET) == 0)
    {
        get_funct(command, &clientSock);
    }
    else if (strcmp(opcode, DELETE) == 0)
    {
        deleteFunct(command, &clientSock);
    }
    else if (strcmp(opcode, PUT) == 0)
    {
        putFunct(command, &clientSock);
    }
    else if (strcmp(opcode, UPDATE) == 0)
    {
        sleep(1);
        updateFunct(command, &clientSock);
    }
    else if (strcmp(opcode, BYE) == 0)
    {
        printf("%s\n", command);
    }

    close(clientSock);
    free(opcode);

    pthread_mutex_lock(&counterMutex);
    thread_count--;
    pthread_mutex_unlock(&counterMutex);

    return NULL;
}

void exit_handle(int signum)
{
    if (signum == SIGTERM || signum == SIGINT)
    {
        printf("Graceful Termination\n");

        for (int i = 0; i < thread_count; i++)
        {
            pthread_join(threadIds[i], NULL);
        }

        destroy_mutexes();
        destroy_FilesInUseStructure();
        exit(0);
    }
}

void set_signals()
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = exit_handle;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int main()
{
    int listenSocket, connectSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLength;
    struct parameter param;

    set_signals();
    init_mutexes();
    init_FilesStructure();
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

    destroy_mutexes();
    destroy_FilesInUseStructure();
    return 0;
}
