#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>


void error_func(int val, char *message)   //error message printer
{	if(val<0)
		printf("%s\n",message);
}

int main()
{
	int fd=open("samplefifo",O_WRONLY);
	write(fd,"hello",15);
}
	