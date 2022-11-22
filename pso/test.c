#include "utils.h"
#include <stdio.h>

int main()
{

	int *a = (int*)malloc(sizeof(int)*4);
	a[2] = 2;

	printf("%d\n",a[2]);

	return 0;
}