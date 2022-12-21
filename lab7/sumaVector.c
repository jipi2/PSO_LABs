#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NR_OF_ELEMENTS 10000

int P;
int N;
int vect[NR_OF_ELEMENTS];

struct params
{
    int begin;
    int n;
};
typedef struct params params_t;

void *thread_func(void *params)
{
    params_t *param = params;
    int *sum = malloc(sizeof(int));
    *sum = 0;
    for(int i=param->begin;i<param->begin+P;i++)
    {
        (*sum) += vect[i];
    }
    return sum;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <no_of_threads> <file_name>", argv[0]);
        exit(-1);
    }

    N = atoi(argv[1]);

    FILE *fin = fopen(argv[2], "r");
    for (int i = 0; i < NR_OF_ELEMENTS; i++)
    {
        fscanf(fin,"%d", &vect[i]);
    }
    fclose(fin);

    P = NR_OF_ELEMENTS/N;
    params_t *parameters = (params_t *)malloc(N * sizeof(params_t));
    pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t)*N);
    for (int i = 0; i < N; i++)
    {
        parameters[i].begin = i*P;
        pthread_create(&tid[i],NULL,thread_func,&parameters[i]);
    }
    
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        int *ret;
       pthread_join(tid[i], (void **)&ret);
       sum += *ret;
    }


    printf("%d\n",sum);

    return 0;
}