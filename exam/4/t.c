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

int main(int argc,char *argv[])
{
	int temp;
	char trainnamebuffer[15];
	sprintf(trainnamebuffer,"%s%s","train",argv[1]);
	printf("%s%s\n",trainnamebuffer," has been created");
	mkfifo(trainnamebuffer,0666);
	int train_fd=open(trainnamebuffer,O_RDONLY|O_NONBLOCK);
	scanf("%d",&temp);

	char platbuffer[15];
	sprintf(platbuffer,"%s%s","platform",argv[1]);
	int train_fd_write=open(platbuffer,O_WRONLY);

	error_func(train_fd_write,"error on opening the train port fifo");
	write(train_fd_write,"ready",10);
	close(train_fd_write);

	struct pollfd fdp[1];
	fdp[0].fd=train_fd;
	fdp[0].events=POLLRDNORM;
	while(1)
	{
		error_func(poll(fdp,1,100),"error on polling");
		if(POLLRDNORM & fdp[0].revents)
		{
			char platform_availablebuffer[15];
			read(train_fd,platform_availablebuffer,sizeof(platform_availablebuffer));

			//send sequence to that platform
			int plat=platform_availablebuffer[4]-'0';
			char infobuffer[15];
			sprintf(infobuffer,"%s%d","platinfo",plat);

			int fd_info=open(infobuffer,O_WRONLY);
			write(fd_info,"sequence",30);

			printf("%s\n","train is on the platform and sleeping");
			sleep(5);
			write(fd_info,"leaving",30);
			close(fd_info);

		}
	}
	
}