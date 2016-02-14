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
char readbuf[100],writebuf[100];

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
	int i;

	fd_set readset;
	FD_ZERO(&readset);
	struct timeval timeptr;
	timeptr.tv_sec=0;
	timeptr.tv_usec=0;

	error_func(mkfifo("fifo",0666),"making fifo server\n");
	fd=open("fifo",O_RDONLY|O_NONBLOCK);
	error_func(fd,"opening file1\n");

	int max=0;
	FD_SET(fd,&readset);
	max =fd;	

	while(1)
	{
		int retstatus=select(max+1,&readset,NULL,NULL,&timeptr);
		error_func(retstatus,"select not working\n");
	
		if(FD_ISSET(fd,&readset))
		{
				read(fd,readbuf,sizeof(readbuf));
				fprintf(stdout,"%s\n",readbuf);
				int c=0;
				int cmp1=strcmp(readbuf,"s1");
				int cmp2=strcmp(readbuf,"s2");

				if(cmp1==0)
				{
					c=fork();
					if(c==0)
					{
						execve("s1",NULL,NULL);
					}
					else
					{
						memset(readbuf,0,sizeof(readbuf));

					}
				}

				else if(cmp2==0)
				{
					c=fork();
					if(c==0)
					{
						execve("s1",NULL,NULL);
					}
					else
					{
						memset(readbuf,0,sizeof(readbuf));

					}	
				}

				memset(readbuf,0,sizeof(readbuf));
					
		}

		FD_SET(fd,&readset);
		max =fd;	

	}
	return 0;
}	