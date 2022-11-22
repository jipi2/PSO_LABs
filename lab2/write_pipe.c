#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
 
#define PIPE_NAME "my_pipe"
#define BUFFER_SIZE 32
 
int main() {
	// Obtinem capatul de scriere
	int pipe_fd = open(PIPE_NAME, O_WRONLY);
 
	char buffer[BUFFER_SIZE];
	strcpy(buffer, "Hello from parent");
 
	// Scriem in pipe
	write(pipe_fd, buffer, strlen(buffer));
 
	close(pipe_fd);
 
	return 0;
}