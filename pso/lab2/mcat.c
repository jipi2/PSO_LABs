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

void write_function_in_file(int fi,int fo, char buff[])
{
    int nr_char = 1; //pentru a intra in while

    while(nr_char)
    {
        nr_char = read(fi,buff,16);
        write(fo,buff,nr_char);
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

    if(fo<0)
        write_function_stdout(fi,buffer);
    else
        write_function_in_file(fi,fo,buffer);

    return 0;
}