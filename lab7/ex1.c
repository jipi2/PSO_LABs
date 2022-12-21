#include <stdio.h>
#include <pthread.h>
 
struct parameters {
  int   param1;
  const char *param2;
};
 
typedef struct parameters parameters_t;
 
void *thread_routine(void *params) {
	parameters_t *args = (parameters_t*) params;
	printf("Received parameters: %i, %p\n", args->param1, args->param2);
 
	return NULL;
}
 
int main()
{
  pthread_t tid;
  parameters_t params;
  params.param1 = 5;
  params.param2 = "hello";
 
  pthread_create(&tid, NULL, thread_routine, &params);
  pthread_join(tid, NULL);
 
  return 0;
}