
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <limits.h>

void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s %d\n",message,val);
		exit(1);
	}
}

int main()
{
	int fd=open("fifo",O_WRONLY);
	error_func(fd,"cannot opne file\n");
	write(fd,"This is from fifo\n",30);
	close(fd);
}
