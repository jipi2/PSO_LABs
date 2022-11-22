#define _XOPEN_SOURCE 700
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void myfree(void *p)
{
	void *handle;

	handle = dlopen("/usr/lib/x86_64-linux-gnu/libc.so.6", RTLD_LAZY);
	if(!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
	}
	dlerror();

	void (*real_free)(void *) = NULL;
	real_free = dlsym(handle,"free");

	fprintf(stdout,"%p\n",p);

	real_free(p);
	return;

}