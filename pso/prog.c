#include "mymath.h"
#define _XOPEN_SOURCE 700
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
 
int main()
{


    void *handle;
    int (*add)(int,int);
    int (*sub)(int,int);
    char *error;
 
    handle = dlopen("libmymath.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();
    add = (int (*)(int,int)) dlsym(handle, "add");
    sub = (int (*)(int,int)) dlsym(handle, "sub");
    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }


  int x, y;
  printf("Introduceti doua numere\n");
  scanf("%d%d", &x, &y);
 
  printf("%d + %d = %d\n", x, y, add(x, y));
  printf("%d - %d = %d\n", x, y, sub(x, y));
 
  dlclose(handle);
  return EXIT_SUCCESS;
}