#define _XOPEN_SOURCE 700
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void* malloc(size_t size)
{
    void * handle;
    handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6",RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();
    void*(* real_malloc)(size_t) = NULL;
    real_malloc = dlsym(handle,"malloc");
    void * p = real_malloc(size);
    fprintf(stdout,"%p\n",p);

    return p;
}