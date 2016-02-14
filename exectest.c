#include <stdio.h>
#include <stdlib.h>

int main()
{
	int c=0;
	c=fork();
	if(c>0)
	{
		wait();
		printf("parent process\n");
	}
	else
	{
		printf("%s\n","child process entered");
		execve("sample",NULL,NULL);
		printf("%s\n","will never be printed");

	}	
	return 0;
}