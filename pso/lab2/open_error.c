#include <fcntl.h>   /* open, O_RDONLY */
#include <errno.h>   /* errno, perror */
#include <stdlib.h>  /* exit */
#include <stdio.h>   /* printf */
 
int main()
{
	int fd = open("non_existing_file", O_RDONLY);
	if (fd < 0)
	{
		printf("The error value is: %d\n", errno);
		perror("The error message is");
		exit(-1);
	}
 
	return 0;
}