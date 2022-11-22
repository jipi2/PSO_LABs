#include <stdio.h>
#include <unistd.h>
 
int main()
{
	int pid;
	setvbuf(stdout, NULL, _IONBF, 0);
 
	printf("<pid:%d>: Executing at line: %d\n", getpid(), __LINE__);
	pid = fork();
	if (pid == 0)
		printf("<pid:%d>: (copil, variabila pid = %d) Executing at line: %d\n", getpid(), pid, __LINE__);
	if (pid > 0)
		printf("<pid:%d>: (parinte, variabila pid = %d) Executing at line: %d\n", getpid(), pid, __LINE__);
 
	printf("<pid:%d>: Executing at line: %d\n", getpid(), __LINE__);
 
	return 0;
}