#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 5

pthread_barrier_t barrier;

void *thread_routine(void *arg)
{
    int thd_id = (int )arg;
    int rc;
    printf("thd %d: before the barrier\n", thd_id);

    // toate firele de execuție așteaptă la barieră.

    rc = pthread_barrier_wait(&barrier);
    if (rc == PTHREAD_BARRIER_SERIAL_THREAD)
    {
        // un singur fir de execuție (posibil ultimul) va întoarce PTHREAD_BARRIER_SERIAL_THREAD
        // restul firelor de execuție întorc 0 în caz de succes.
        printf("   let the flood in\n");
    }
    printf("thd %d: after the barrier\n", thd_id);
    return NULL;
}
int main(void)
{
    int i;
    pthread_t tids[NUM_THREADS];
    // bariera este inițializată o singură dată și folosită de toate firele de execuție
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    // firele de execuție vor executa codul funcției 'thread_routine'.
    // în locul unui pointer la date utile, se trimite în ultimul argument
    // un întreg - identificatorul firului de execuție
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tids[i], NULL, thread_routine, (void *)i);
    // așteptăm ca toate firele de execuție să se termine
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tids[i], NULL);
    // eliberăm resursele barierei
    pthread_barrier_destroy(&barrier);
    return 0;
}