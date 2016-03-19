#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void handler_func()
{
	printf("%s\n","hi how are you from signal");
}
int main()
{
	printf("%s\n","starting");
	signal(SIGUSR1,handler_func);
	printf("%d\n",getpid());
	while(1);

	printf("%s\n","sleeping");
	sleep(3);
	raise(SIGUSR1);
	printf("%s\n","sleeping agaian");
	sleep(4);
	kill(getpid(),SIGUSR1);
}