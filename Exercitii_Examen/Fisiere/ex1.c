#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buffer[16];

    int rfd = open("in.txt",O_RDONLY);
    int wfd=0;
    int bytesRed = 0;

    dup2(wfd, STDOUT_FILENO);

    do
    {
        bytesRed = read(rfd, buffer, 16);
        buffer[bytesRed] = '\0';

        write(wfd, buffer, bytesRed);

    } while (bytesRed > 0);
    

    return 0;
}