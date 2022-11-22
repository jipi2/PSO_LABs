#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int dev = open("/dev/mydevice",O_RDONLY);
    if(dev == -1)
        {printf("Something is eroare\n");
            return -1;
        }
    printf("Totul a mers fara eroare\n");
    close(dev);
    return 0;
}