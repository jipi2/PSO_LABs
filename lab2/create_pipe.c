#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
 
#define PIPE_NAME "my_pipe"
 
int main() {
	int ret;
	ret = mkfifo(PIPE_NAME, 0644);
	if (ret < 0) {
		perror("error on create pipe: ");
	}
 
	return 0;
}