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
int main(int argc,char * argv[])
{
	char buf[30];
	int i;
	printf("%s\n","s1 is called");
	fd=open("fifoserver",O_WRONLY,0666);
	error_func(fd,"opening file");
	int a,b;
	a=atoi(argv[1]);
	b=atoi(argv[2]);
	a=a+b;
	sprintf(buf,"%d",a);
	write(fd,buf,30);
	close(fd);
	return 0;
}