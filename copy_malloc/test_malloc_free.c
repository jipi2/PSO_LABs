#include "utils.h"
#include <stdio.h>

int main()
{

	myfree(mymalloc(200));

	return 0;
}