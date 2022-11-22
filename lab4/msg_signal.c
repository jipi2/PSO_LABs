#include <stdio.h>
 
/*https://man7.org/linux/man-pages/man7/feature_test_macros.7.html*/
#define __USE_GNU
#include <string.h>
 
#include <signal.h>
 
int main()
{
	printf("Semnalul %d este descris de  %s\n", SIGTTOU, strsignal(SIGTTOU));
	psignal(SIGTTOU, "Afisam un mesaj si un string care descrie semnalul");
	printf("Semnalul %d are abrevierea %s\n", SIGTTOU, sigabbrev_np(SIGTTOU));
}
