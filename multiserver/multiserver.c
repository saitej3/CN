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
	error_func(mkfifo("fifo",0666),"making fifo server\n");
	fd=open("fifo",O_RDONLY|O_NONBLOCK);
	error_func(fd,"opening file1\n");

	struct pollfd fdp[1];
	fdp[0].fd=fd;
	fdp[0].events=POLLRDNORM;

	while(1)
	{
		error_func(poll(fdp,1,100),"poll error");
	
		if(fdp[0].revents & fdp[0].events)
		{
				read(fd,readbuf,sizeof(readbuf));
				fprintf(stdout,"%s\n",readbuf);

				const char s[2] = "|";
				char *token;
   				token = strtok(readbuf, s);
   				printf("%s\n",token );
				int c=0;
				int cmp1=strcmp(token,"s1");
				int cmp2=strcmp(token,"s2");

				if(cmp1==0)
				{
					c=fork();
					if(c==0)
					{
						execv("s1",NULL);
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
						char * argv[10];
						int i = 0;
						while( token != NULL ) 
						{
							argv[i] = (char*) malloc(20);
							sprintf(argv[i++], "%s", token);
				  			token = strtok(NULL, s);
				  			
						}
						argv[i] = NULL;
						execv("s1",argv);
					}
					else
					{
						memset(readbuf,0,sizeof(readbuf));

					}	
				}

				memset(readbuf,0,sizeof(readbuf));
					
		}

	}
	return 0;
}	