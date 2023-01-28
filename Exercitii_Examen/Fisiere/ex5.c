/* 5. Scrieți o aplicație care deschide un fișier doar pentru citire, 
citește într-un buffer conținutul(care va avea dimeniunea de max 1024 bytes)
, schimbă modul în O_WRONLY și adaugă flagul O_NOATIME descriptorului 
inițial, duplică conținutul bufferului în fișier iar apoi își încheie 
execuția. Vezi fcntl. */
#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    char buffer[1024];
    int fd = 0;
    int byteswrote = 0;
    int bytesRead = 0;
    int flags = 0;

    fd = open("in5.txt", O_RDONLY);
    if(fd < 0)
    {
        printf("Eroare la deschiderea fisierului\n");
        exit(-1);
    }

    bytesRead = read (fd, buffer, 1024);
    buffer[bytesRead] = '\0';
    printf("%d\n", bytesRead);


    flags = fcntl(fd, F_GETFL);

    if(flags < 0)
    {
        printf("Flagurile nu sunt culese bine de fnctl()\n");
        exit(-1);
    }

    
    flags = (flags & ~O_ACCMODE ) | O_WRONLY | O_NOATIME;

    fcntl(fd, F_SETFL, flags);
    if(flags < 0)
    {
        printf("Flagurile nu sunt PUSE bine de fnctl()\n");
        exit(-1);
    }

    byteswrote=write(fd, buffer, 1024);
    printf("%d\n", byteswrote);

    close(fd);

    return 0;
}