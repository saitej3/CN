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
	mkfifo("samplefifo",0666);
	int fd=open("samplefifo",O_RDONLY|O_NONBLOCK);
	struct pollfd fdp[1];
	fdp[0].fd=fd;
	fdp[0].events=POLLRDNORM;
	char buffer[15];
	while(1)
	{
		error_func(poll(fdp,1,0),"error on polling");
		if(fdp[0].events & fdp[0].revents)
		{
			read(fd,buffer,sizeof(buffer));
			printf("%s\n",buffer );
		}
	}

}