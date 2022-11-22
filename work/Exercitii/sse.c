#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#ifdef __i386__
#define BITMASK (0xFFFFFF00ul)
#else
#define BITMASK (0xFFFFFFFFFFFFFF00ull)
#endif

void normal(float* a, int N)
{
	for (int i = 0; i < N; ++i)
		a[i] = sqrt(a[i]);
}

int main()
{
	float MAX = 5;
	unsigned int N = 1024  * 1024 * 120;
	
	clock_t start, end;
	double cpu_time_used;
	
	srand((unsigned int)time(NULL));
	
	float *a_init_normal = malloc(N * sizeof(float) + 255);
	float *a_init_sse = malloc(N * sizeof(float) + 255);
	
	float *a_normal =(float*)(((uintptr_t)a_init_normal + 255) & BITMASK);
	float *a_sse = (float*)(((uintptr_t)a_init_sse + 255) & BITMASK);

	start = clock();
		for (int i = 0; i < N; i++)
		a_normal[i] = ((float)rand() / (float)(RAND_MAX)) * MAX;
	memcpy(a_sse, a_normal, N * sizeof(float));
	end = clock();

	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("generating random numbers took %f seconds to execute \n", cpu_time_used);

	start = clock();
	normal(a_normal, N);
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("normal() took %f seconds to execute \n", cpu_time_used);

	start = clock();
	// TODO:
	end = clock();
	
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("sse() took %f seconds to execute \n", cpu_time_used);

	if (!memcmp(a_normal, a_sse, N * sizeof(float)))
		printf("OK:Identical!\n");
	else
		printf("NOK:Non Identical!\n");


	free(a_init_normal);
	free(a_init_sse);
	system("pause");


	return 0;
}
