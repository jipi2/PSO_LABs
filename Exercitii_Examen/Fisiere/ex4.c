/* 4. Implementați un program care primește ca parametru un fișier 
pentru scriere. Dacă fișierul există și dimensiunea lui este mai mare 
de 10 octeți, conținutul său va fi trunchiat la 10 octeți. Dacă există 
și sre dimensiunea mai mică de 10 octeți va rămâne la dimensiunea sa 
inițială. Dacă nu există va fi creat. Aplicația va scrie 10 litere random 
în fișier și își va încheia execuția. */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void write_in_file(int fd, int size)
{
    char buffer[10];
    char c='A';

    for(int i=0;i<size;i++)
    {
        buffer[i] = c++;
    }
    buffer[size] = '\0';
    write(fd, buffer, size);
}

int main(int argc, char *argv[])
{
    int fd = 0;
    char path[50];
    int size = 0;

    if(argc < 2)
    {
        printf("Nu sunt destui paramteri\n");
        exit(-1);
    }

    strcpy(path, argv[1]);

    fd = open(path, O_RDONLY);
    if(fd > 0)
    {   
        printf("aici\n");
        close(fd);
        fd = open(path, O_RDWR);
        size = lseek(fd, 0, SEEK_END);
        if(size > 10)
        {   
            lseek(fd, 0, SEEK_SET);
            ftruncate(fd, 10);
        }
        lseek(fd, 0, SEEK_SET);
        write_in_file(fd, size);
        close(fd);
    }
    else
    {
        //printf("aici\n");
        fd = open(path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        size = 10;
        ftruncate(fd, size);
        write_in_file(fd, size);
        close(fd);
    }


    return 0;
}