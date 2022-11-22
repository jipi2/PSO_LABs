#include <stdio.h>
#include <fcntl.h> /* open, O_RDWR, O_CREAT, O_TRUNC */
#include <unistd.h>

#define SIZE 16

void write_function_stdout(int fd, char buff[])
{
    int nr_char = 1; //pentru a intra in while

    while(nr_char)
    {
        nr_char = read(fd,buff,16);
        write(1,buff,nr_char);
    }
}

int main(int argc, char *argv[])
{
    char buffer[SIZE];

    int fi = open(argv[1],O_RDONLY);

    if(fi<0)
    {
        perror("Error opening input file\n");
        return -1;
    }


    int fo = open(argv[2],O_WRONLY | O_CREAT, 0640);

    if(fo>0)
    {
        dup2(fo,1);
    }

    int nr_char = 1; //pentru a intra in while

    while(nr_char)
    {
       
        nr_char = read(fi,buffer,16);
        write(1,buffer,nr_char);
    }

    return 0;
}