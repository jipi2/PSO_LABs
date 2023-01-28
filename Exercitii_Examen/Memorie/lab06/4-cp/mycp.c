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
	int size = 0;

	DIE(argc != 3, "Usage: ./mycp <from_file> <to_file>");

	/* Open input and output files */
	fdin = open(argv[1], O_RDONLY);
	DIE(fdin == -1, "open fdin");

	/* Open/create the output file */
	fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
	DIE(fdout == -1, "open fdout");

	/* TODO - Truncate the output file to the input file size */
	fstat(fdin, &statbuf);
	size = statbuf.st_size;
	ftruncate(fdout, size);

	/* TODO - mmap the input and output file */
	src = mmap(0,size, PROT_READ, MAP_SHARED, fdin, 0);
	dst = mmap(0,size, PROT_WRITE, MAP_SHARED, fdout, 0);

	/* TODO - Copy the input file to the output file */
	memcpy(dst, src, size);

	/* TODO - Clean up */
	munmap(src, size);
	munmap(dst, size);

	/* Close files */
	rc = close(fdin);
	DIE(rc == -1, "close source");

	rc = close(fdout);
	DIE(rc == -1, "close destination");

	return 0;
}
