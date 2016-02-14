#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

int fdA[10],fdB;
char readbuf[100],writebuf[100];
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
	int i;
	printf("%d\n",argc);
	for(i=0;i<argc-1;i++)
	{
		char file[50];
		memset(file,0,50);
		strcat(file,argv[i+1]);
		strcat(file,"B");
		error_func(mkfifo(file,0666),"making fifo server\n");
		fdA[i]=open(file,O_RDONLY|O_NONBLOCK);
		error_func(fdA[i],"opening file1\n");
	}

	struct pollfd fdp[argc-1];
	for(i=0;i<argc-1;i++)
	{
		fdp[i].fd=fdA[i];
		fdp[i].events=POLLRDNORM;
	}

	int j;
	while(1)
	{
		error_func(poll(fdp,argc-1,100),"poll error");
		for(i=0;i<argc-1;i++)
		{
			if(fdp[i].revents & fdp[i].events)
			{
				read(fdA[i],readbuf,sizeof(readbuf));
				fprintf(stdout,"%s\n",readbuf);
				memset(readbuf,0,sizeof(readbuf));
			}

		}
	}

	return 0;
}