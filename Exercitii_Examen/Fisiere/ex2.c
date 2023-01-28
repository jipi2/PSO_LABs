#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int rfd = open("in.txt", O_NOFOLLOW | O_RDONLY);
    char buffer [20];
    int bytesRed = 0;

    lseek(rfd, -20, SEEK_END);

    bytesRed = read(rfd, buffer, 20);
    buffer[bytesRed] = '\0';

    printf("%s\n", buffer);

    return 0;
}