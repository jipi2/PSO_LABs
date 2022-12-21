/*
 * Loader Implementation
 *
 * 2022, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "exec_parser.h"

#define OK 1
#define N_OK -1

static so_exec_t *exec;
static struct sigaction *default_action;
static int exec_descriptor = 0;

static int Get_Index_Segment(void *page_addr)
{
	for (int i = 0; i < exec->segments_no; i++)
	{
		if ((int *)page_addr >= (int *)exec->segments[i].vaddr && ((int)exec->segments[i].vaddr + exec->segments[i].mem_size) >= (int)page_addr) // merge si asa
			return i;
	}

	return N_OK; // inseamna ca adresa nu este in niciun segment
}

void Map_to_Memory(void *page_mapped, so_seg_t *segment, size_t offset)
{
	int page_size = getpagesize();
	char *pSrc = NULL;

	pSrc = mmap(0, page_size, PROT_READ, MAP_SHARED, exec_descriptor, segment->offset + offset);

	if (segment->file_size >= offset + page_size)
		memcpy(page_mapped, pSrc, page_size);

	else if (segment->file_size >= offset)
	{
		memcpy(page_mapped, pSrc, segment->file_size - offset);
	}

	else if (segment->file_size < offset)
		memset(page_mapped, 0, page_size);
}

static void segv_handler(int signum, siginfo_t *info, void *context)
{
	/* TODO - actual loader implementation */
	void *seg_addr;
	int page_size = getpagesize();
	int segment_index = 0;
	void *page_start_addr = NULL;
	so_seg_t *segment = NULL;
	void *page_mapped = NULL;
	size_t segm_offset = 0;
	size_t page_offset = 0;

	if (signum != SIGSEGV)
	{
		default_action->sa_sigaction(signum, info, context);
		return;
	}

	if (info->si_code == SEGV_ACCERR)
	{
		default_action->sa_sigaction(signum, info, context);
		return;
	}

	seg_addr = info->si_addr;
	page_start_addr = (void *)ALIGN_DOWN((int)seg_addr, page_size);
	segment_index = Get_Index_Segment(seg_addr);
	if (segment_index == N_OK)
	{
		default_action->sa_sigaction(signum, info, context);
		return;
	}

	segment = &exec->segments[segment_index];

	page_mapped = mmap(page_start_addr,
					   page_size,
					   PERM_R | PERM_W | PERM_X,
					   MAP_SHARED | MAP_ANONYMOUS,
					   -1,
					   0);

	segm_offset = (char *)info->si_addr - (char *)segment->vaddr;
	page_offset = segm_offset % page_size;
	segm_offset -= page_offset;

	Map_to_Memory(page_mapped, segment, segm_offset);
	mprotect(page_mapped, page_size, segment->perm);
}

int so_init_loader(void)
{
	int rc;
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));

	sa.sa_sigaction = segv_handler;
	sa.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &sa, default_action);
	if (rc < 0)
	{
		perror("sigaction");
		return -1;
	}
	return 0;
}

int so_execute(char *path, char *argv[])
{
	exec_descriptor = open(path, O_RDONLY);

	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	close(exec_descriptor);

	return -1;
}
