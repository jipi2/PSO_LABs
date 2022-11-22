#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <sys/types.h>	/* open */
#include <sys/stat.h>	/* open */
#include <fcntl.h>	/* O_CREAT, O_RDONLY */
#include <unistd.h>	/* close, lseek, read, write */

int main(int agrc, char *argv[])
{
    int fd;
    const char *ptr;
    fd = open((const char *)ptr, O_RDONLY);

    if(fd < 0)
    {
        fprintf(stderr, "Failed to open input device %s: %s\n",(const char*)ptr, strerror(errno));
    }

    return 0;
}