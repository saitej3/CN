#include<stdio.h>
#include<stdlib.h>

int main()
{
	printf("%d\n",getpid());
	printf("%s\n","forking");
	int c=0;
	c=fork();
	if(c==0)
	{
		printf("%s%d","from child process",getppid());
		printf("child process\n");
		printf("%d\n",getpid());
	}
	else
	{
		printf("%s\n","parent process");
		printf("%d\n",getpid());
	}
}

