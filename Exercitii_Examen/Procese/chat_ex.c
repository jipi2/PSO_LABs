#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    // Create the named pipe
    int res = mkfifo("mfifo", 0666);
   /*  if (res != 0) {
        perror("mkfifo error");
        exit(1);
    } */

    // Create the child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        int fd = open("mfifo", O_RDONLY);
        if (fd < 0) {
            perror("open error");
            exit(1);
        }

        char buf[100];
        int n = read(fd, buf, 100);
        if (n < 0) {
            perror("read error");
            exit(1);
        }

        printf("Child process read: %s\n", buf);
        close(fd);
    } else {
        // Parent process
        int fd = open("mfifo", O_WRONLY);
        if (fd < 0) {
            perror("open error");
            exit(1);
        }

        char* message = "hello";
        int n = write(fd, message, strlen(message));
        if (n < 0) {
            perror("write error");
            exit(1);
        }

        close(fd);
    }

    return 0;
}
