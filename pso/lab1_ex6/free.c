#define _XOPEN_SOURCE 700
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void free(void* ptr)
{
    void * handle;
    handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6",RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();
    void(* real_free)(void* ) = NULL;
    real_free = dlsym(handle,"free");
    fprintf(stdout,"%p\n",ptr);
    real_free(ptr);
    return;
}