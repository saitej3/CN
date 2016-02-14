#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
int fd;
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
	printf("%s\n","s1 is called");
	fd=open("fifoserver",O_WRONLY,0666);
	error_func(fd,"opening file");
	write(fd,"sending to client",30);
	close(fd);
	return 0;
}