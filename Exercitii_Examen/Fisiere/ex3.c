#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char path[50];
    char permissions[50];
    int rv = 0;
    int fd = 0;

    if(argc < 3)
    {
        printf("Nu sunt destui parametri\n");
        exit(-1);
    }

    strcpy(path, argv[1]);
    strcpy(permissions, argv[2]);

    fd = open(path, O_EXCL);
    if(fd > 0)
    {
        printf("Fisierul exista deja\n");
        close(fd);
        exit(-1);
    }

    mode_t mode = 0;

    if (strlen(permissions) == 9) {
        if (permissions[0] == 'r') mode |= S_IRUSR;
        if (permissions[1] == 'w') mode |= S_IWUSR;
        if (permissions[2] == 'x') mode |= S_IXUSR;
        if (permissions[3] == 'r') mode |= S_IRGRP;
        if (permissions[4] == 'w') mode |= S_IWGRP;
        if (permissions[5] == 'x') mode |= S_IXGRP;
        if (permissions[6] == 'r') mode |= S_IROTH;
        if (permissions[7] == 'w') mode |= S_IWOTH;
        if (permissions[8] == 'x') mode |= S_IXOTH;
    }

    fd = creat(path, mode);
    if(fd < 0)
    {
        printf("Nu s-a create fisierul, nu stim motivul\n");
        exit(-1);
    }

    printf("Fisierul s-a creat cu succes\n");

    close(fd);

    return 0;
}