/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #4, Linux
 *
 * Implementing 'cp' with mmap
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

int main(int argc, char *argv[])
{
	int fdin;
	int fdout;
	int rc;
	char *src;
	char *dst;
	struct stat statbuf;
	char buffer[1024];

	DIE(argc != 3, "Usage: ./mycp <from_file> <to_file>");

	/* Open input and output files */
	fdin = open(argv[1], O_RDONLY);
	DIE(fdin == -1, "open fdin");

	/* Open/create the output file */
	fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
	DIE(fdout == -1, "open fdout");

	/* TODO - Truncate the output file to the input file size */
	rc = fstat(fdin, &statbuf);
	if(rc ==-1)
		exit(-1);

	rc = ftruncate(fdout, statbuf.st_size);
	if(rc == -1)
		exit(-1);
	

	/* TODO - mmap the input and output file */
	src = mmap(0,statbuf.st_size,PROT_READ, MAP_SHARED,fdin,0);
	dst = mmap(0,statbuf.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,fdout,0);

	/* TODO - Copy the input file to the output file */
	memcpy(dst,src,statbuf.st_size);
	memcpy(buffer,src,statbuf.st_size);


	/* TODO - Clean up */
	munmap(src,statbuf.st_size);
	munmap(dst,statbuf.st_size);
	
 
	/* Close files */
	rc = close(fdin);
	DIE(rc == -1, "close source");

	rc = close(fdout);
	DIE(rc == -1, "close destination");

	return 0;
}
