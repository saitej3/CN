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

struct pollfd fdp[5];
char readbuf[30];
void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s %d\n",message,val);
		exit(1);
	}
}

void pipeFunc()
{
	int fd[2];
	error_func(pipe(fd),"Creating the pipe\n");
	int c=fork();
	error_func(c,"forking the process\n");
	if(c==0)
	{
		//child process
		close(fd[0]);
		sleep(10);
		write(fd[1],"From process p1\n",30);
		exit(0);
	}
	else
	{
		//parent process
		close(fd[1]);
		fdp[0].fd=fd[0];
		fdp[0].events=POLLRDNORM;
	}
}


int main()
{
	char filenamebuffer[15];
	int i,j;

	for(i=2;i<=5;i++)
	{
		sprintf(filenamebuffer,"%s%d","./popen",i);
		FILE * f=popen(filenamebuffer,"r");
		fdp[i-1].fd=fileno(f);
		fdp[i-1].events=POLLRDNORM;
	}

	pipeFunc();

	while(1)
	{
		error_func(poll(fdp,5,100),"poll error");
		for(i=0;i<5;i++)
		{
			if(fdp[i].revents & fdp[i].events)
			{
				read(fdp[i].fd,readbuf,sizeof(readbuf));
				//FILE * f=popen("./popen6","w");
				//int fd=fileno(f);
				//write(fd,readbuf,sizeof(readbuf));
				fprintf(stdout,"%s\n",readbuf);
				memset(readbuf,0,sizeof(readbuf));				
			}

		}
	}

	
}