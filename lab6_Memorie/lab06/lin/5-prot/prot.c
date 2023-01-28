/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #5, Linux
 *
 * Changing page access protection
 */
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"

static int pageSize;
static struct sigaction old_action;
char *p;
int how[3] = {PROT_NONE, PROT_READ, PROT_WRITE};

static void segv_handler(int signum, siginfo_t *info, void *context)
{
	char *addr;
	int rc;

	/**
	 * Calling the old signal handler by default for TODO 1
	 * Comment the line below when solving TODO 2
	 */
	//old_action.sa_sigaction(signum, info, context);

	/* TODO 2 - Check if the signal is SIGSEGV */

	/**
	 * TODO 2 - Obtain from siginfo_t the memory location
	 * which caused the page fault
	 */

	/**
	 * TODO 2 - Obtain the page which caused the page fault
	 * Hint: use the address returned by mmap
	 */

	/* TODO 2 - Increase protection for that page */

	if(signum == SIGSEGV)
	{
		addr = info->si_addr;
		if(addr == p)
		{
			rc = mprotect(addr,pageSize,PROT_READ);
			if(rc == -1)
				exit(-1);
		}
		else if (addr == p+pageSize || addr == p+pageSize*2)
		{
			rc = mprotect(addr,pageSize,PROT_READ | PROT_WRITE);
			if(rc == -1)
				exit(-1);
		}
	}
}

static void set_signal(void)
{
	struct sigaction action;
	int rc;

	action.sa_sigaction = segv_handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, &old_action);
	DIE(rc == -1, "sigaction");
}

static void restore_signal(void)
{
	struct sigaction action;
	int rc;

	action.sa_sigaction = old_action.sa_sigaction;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, NULL);
	DIE(rc == -1, "sigaction");
}

int main(void)
{
	int rc;
	//char ch;
	char buffer[256];
	int bufferLenght;

	pageSize = getpagesize();

	/**
	 * TODO 1 - Map 3 pages with the desired memory protection
	 * Use global 'p' variable to keep the address returned by mmap
	 * Use mprotect to set memory protections based on global 'how' array
	 * 'how' array keeps protection level for each page
	 */
	p = mmap(0,pageSize*3,PROT_NONE, MAP_ANONYMOUS | MAP_SHARED,-1,0);
	/* rc = mprotect(p+pageSize,pageSize,PROT_READ);

	if(rc == -1)
		exit(-1);

	rc = mprotect(p+pageSize*2,pageSize,PROT_WRITE);
	if(rc == -1)
		exit(-1); */

	for(int i=0;i<3;i++)
	{
		mprotect(p+i*pageSize, pageSize, how[i]);
	}

	set_signal();

	/* TODO 1 - Access these pages for read and write */

	strcpy(buffer,"Ana are mere si pere");
	bufferLenght = strlen(buffer);

	memcpy(p+pageSize,buffer,bufferLenght);
	strcpy(buffer,"");
	memcpy(buffer,p+pageSize,bufferLenght);
	buffer[bufferLenght] = '\0';
	printf("%s\n",buffer);

	strcpy(buffer,"Ana are mere si pere");
	memcpy(p+pageSize*2,buffer,bufferLenght);
	strcpy(buffer,"");
	memcpy(buffer,p+pageSize*2,bufferLenght);
	buffer[bufferLenght] = '\0';
	printf("%s\n",buffer); 

	/* char ch;
	for(int i=0;i<3;i++)
	{
		/* ch = p[i*pageSize+100];
		p[i*pageSize+120] = 'a'; 
		ch = p[pageSize*i];
		p[i*pageSize] = 'a';
	} */


	restore_signal();

	/* TODO 1 - Unmap */
	munmap(p,pageSize*3);

	return 0;
}
