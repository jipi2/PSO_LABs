//Afisati de cate ori apare un element intr-un vector si afisati si indexii unde apare

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

pthread_mutex_t muflex;
int counter = 0;
char buffer[256];
int vect[] = {12, 1, 23, 12, 32, 323, 1, 12, 3, 23, 12, 12, 3, 1};

struct parameter
{
    int begin;
    int end;
    int wantedNumber;
};

void *work_for_master(void *param)
{
    struct parameter *par = (struct parameter*) param;
    int i=0;
    char number[8];

    for(i=par->begin;i<par->end;i++)
    {
        if(vect[i] == par->wantedNumber)
        {
            pthread_mutex_lock(&muflex);
            counter++;
            sprintf(number, "%d", i);
            strcat(buffer, number);
            strcat(buffer, " ");
            pthread_mutex_unlock(&muflex);
        }
    }
}

int main()
{
    int sizeVect = 14;
    pthread_t *tids = NULL;
    int numOfThreads = 4;
    struct parameter *params = NULL;
    int chunk = 0;
    int i=0;
    int wanted = 12;

    strcpy(buffer,"");

    pthread_mutex_init(&muflex, NULL);
    tids = (pthread_t *)malloc(sizeof(pthread_t)*numOfThreads);
    params = (struct parameter *)malloc(sizeof(struct parameter)*numOfThreads);

    chunk = sizeVect/numOfThreads;

    for(i=0;i<numOfThreads;i++)
    {   
        params[i].wantedNumber = wanted;
        params[i].begin = i*chunk;
        if(i == numOfThreads - 1)
        {
            params[i].end = sizeVect;
        }
        else
            params[i].end = params[i].begin+chunk;
        
        pthread_create(&tids[i], NULL, &work_for_master, &params[i]);
    }


    for(i=0;i<numOfThreads;i++)
    {
        pthread_join(tids[i],NULL);
    }

    printf("S-au gasit %d aparitii, ale numarului %d!\nPozitiile sunt:\n", counter, wanted);
    printf("%s\n", buffer);

    free(tids);
    free(params);
    pthread_mutex_destroy(&muflex);

    return 0;
}