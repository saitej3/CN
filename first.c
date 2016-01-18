#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	int c=0;
	int fd[2];
	pipe(fd);
	c=fork();
	if(c==0)
	{
		close(fd[1]);
		char buf[128];
		read(fd[0],buf,128);
		printf("%s\n",buf);
		
	}
	else
	{
		close(fd[0]);
		char buf[128]="hello world\n";
		write(fd[1],buf,128);
	}
}
