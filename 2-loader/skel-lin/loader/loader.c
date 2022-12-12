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

#include "exec_parser.h"

#define OK 1
#define N_OK -1

static so_exec_t *exec;
static struct sigaction *default_action;
static int exec_descriptor = 0;

typedef struct PAGE_LIST
{
	void *page_addr;
	struct PAGE_LIST *next;

}Page_list;

static struct PAGE_LIST *head = NULL;

void Add_Page(struct PAGE_LIST**head,void *addr)
{
	struct PAGE_LIST *new_node = (struct PAGE_LIST *)malloc(sizeof(struct PAGE_LIST));
	new_node->page_addr = addr;
	new_node->next = NULL;

	if(*head == NULL)
	{
		*head = new_node;
		return;
	}

	struct PAGE_LIST *it_node = *head;

	while(it_node->next != NULL)
		it_node = it_node->next;

	it_node->next = new_node;

}

int Exists_In_List(struct PAGE_LIST *head, void *page_addr)
{
	while(head != NULL)
	{
		if(((char*)page_addr - (char*)head->page_addr) < getpagesize() && (page_addr - head->page_addr) >=0)
			return OK;
	}
	return N_OK;
}

static int Get_Index_Segment(void *page_addr)
{
	for(int i=0;i<exec->segments_no;i++)
	{
		if(page_addr >= exec->segments[i].vaddr && (exec->segments[i].vaddr + exec->segments[i].mem_size) >= page_addr)
			return i;
	}
	
	return -1; //inseamna ca nu adresa nu este in niciun segment
}

size_t My_Fread(void *buffer, size_t bytes_to_be_read)
{
	size_t bytes_read = 0;
	size_t count = 0;

	while(bytes_read < bytes_to_be_read)
	{
		count = read(exec_descriptor, buffer+bytes_read, bytes_to_be_read-bytes_read);

		if(count == -1)
			return -1;

		if(count == 0)
			return bytes_read;

		bytes_read += count;
		count = 0;
	}
	return bytes_read;
}

void From_File_to_Memory(void *page_mapped, so_seg_t *segment, size_t offset)
{
	int page_size = getpagesize();
	char buffer[page_size];

	lseek(exec_descriptor, segment->offset + offset, SEEK_SET);

	if(segment->file_size >= offset+page_size)
	{
		My_Fread(buffer, page_size);
		memcpy(page_mapped, buffer, page_size);
	}
	else if (segment->file_size >= offset)
	{
		My_Fread(buffer, segment->file_size - offset);
		memset(buffer+segment->file_size-offset, 0, page_size+offset-segment->file_size);
		memcpy(page_mapped, buffer, page_size);
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

	if(signum != SIGSEGV)
	{
		default_action->sa_sigaction(signum, info, context);
		return;
	}

	seg_addr = info->si_addr;
	page_start_addr = ALIGN_DOWN((int)seg_addr, page_size);
	segment_index = Get_Index_Segment(seg_addr);
	if(segment_index == -1)
	{
		default_action->sa_sigaction(signum, info, context);
		return;
	}

	segment = &exec->segments[segment_index];

	if(Exists_In_List(head, seg_addr) == OK)
	{
		default_action->sa_sigaction(signum, info, context);
			return;
	}
	
	Add_Page(&head, page_start_addr);

	page_mapped = mmap(page_start_addr,
						page_size, 
						PERM_R | PERM_W | PERM_X,
						MAP_SHARED | MAP_ANONYMOUS,
						-1,
						0
						);
	
	segm_offset = (char *)info->si_addr - (char *)segment->vaddr;
	page_offset = segm_offset % page_size;
	segm_offset -= page_offset;
	From_File_to_Memory(page_mapped, segment, segm_offset);

}

int so_init_loader(void)
{
	int rc;
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));

	sa.sa_sigaction = segv_handler;
	sa.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &sa, default_action);
	if (rc < 0) {
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

	return -1;
}
