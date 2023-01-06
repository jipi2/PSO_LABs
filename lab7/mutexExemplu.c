#include <stdio.h>
#include <pthread.h>
 
#define NUM_THREADS 5
 
/* global mutex */
pthread_mutex_t mutex;
int global_counter = 0;
 
void *thread_routine(void *arg) 
{    
    /* acquire global mutex */
    pthread_mutex_lock(&mutex);
 
    /* print and modify global_counter */
    printf("Thread %d says global_counter=%d\n", (int) arg, global_counter);
    global_counter++;
 
    /* release mutex - now other threads can modify global_counter */
    pthread_mutex_unlock(&mutex);
 
    return NULL;
}
 
int main(void) 
{
    int i;
    pthread_t tids[NUM_THREADS];
 
    /* init mutex once, but use it in every thread */
    pthread_mutex_init(&mutex, NULL);
 
    /* all threads execute thread_routine
       as args to the thread send a thread id 
       represented by a pointer to an integer */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tids[i], NULL, thread_routine, (void *) i);
 
    /* wait for all threads to finish */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tids[i], NULL);
 
    /* dispose mutex */
    pthread_mutex_destroy(&mutex);
 
    return 0;
}