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

int fdp[4]; //keyboard,pipe,fifo,popen

char readbuf[100],writebuf[100];
void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s %d\n",message,val);
		exit(1);
	}
}


void keyboardFunc()
{
	fdp[0]=0;
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
		write(fd[1],"From Child Pipe",30);
		exit(0);
	}
	else
	{
		//parent process
		close(fd[1]);
		fdp[1]=fd[0];
	}
}

void signalFunc(int signo)
{
	printf("%s\n","Siganl caught");
}
void fifoFunc()
{
	error_func(mkfifo("fifo",0666),"making fifo server\n");
	int fd=open("fifo",O_RDONLY|O_NONBLOCK);
	error_func(fd,"opening file\n");
	fdp[2]=fd;
		
}

void popenFunc()
{
	FILE * f=popen("./popenproc","r");
	fdp[3]=fileno(f);
}
int main(int argc,char * argv[])
{
	int i;
	printf("%d\n",argc);

	//select
	fd_set readset;
	FD_ZERO(&readset);
	struct timeval timeptr;
	timeptr.tv_sec=0;
	timeptr.tv_usec=0;
	//doneselect
	int max=0;
	for(i=0;i<argc-1;i++)
	{
		FD_SET(fdp[i],&readset);
		if(fdp[i] > max)
			max=fdp[i];
	}

	popenFunc();
	keyboardFunc();
	pipeFunc();
	fifoFunc();
	signal(SIGUSR1,signalFunc);
	int j;
	while(1)
	{
		int retstatus=select(max+1,&readset,NULL,NULL,&timeptr);
		error_func(retstatus,"select not working\n");
		for(i=0;i<4;i++)
		{
			if(FD_ISSET(fdp[i],&readset))
			{
				read(fdp[i],readbuf,sizeof(readbuf));
				fprintf(stdout,"%s\n",readbuf);
				memset(readbuf,0,sizeof(readbuf));				
			}

		}

		for(i=0;i<argc-1;i++)
		{
			FD_SET(fdp[i],&readset);
			if(fdp[i] > max)
				max=fdp[i];
		}
	}

	return 0;
}