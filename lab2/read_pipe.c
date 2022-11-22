#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
 
#define PIPE_NAME "my_pipe"
#define BUFFER_SIZE 32
 
int main() {
	// Obtinem capatul de citire
	int pipe_fd = open(PIPE_NAME, O_RDONLY);
 
	char buffer[BUFFER_SIZE];
 
	// Citim din pipe
	int ret = read(pipe_fd, buffer, BUFFER_SIZE);
 
	buffer[ret] = '\0';
 
	printf("Received: %s\n", buffer);
 
	close(pipe_fd);
 
	return 0;
}