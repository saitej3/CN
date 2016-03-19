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
#include <sys/ipc.h>
#include <sys/shm.h>

void error_func(int val, char *message)   //error message printer
{	if(val<0)
		printf("%s\n",message);
}

int plat_available[3];   //platform available index;

void broadcast_plat(char * message)
{
	printf("%s\n",message);
	int i;
	for(i=0;i<3;i++)
	{
		char tempbuffer[15];
		sprintf(tempbuffer,"%s%d","platbrod",i+1);
		int fd_temp=open(tempbuffer,O_WRONLY);
		write(fd_temp,"train coming",15);
		close(fd_temp);
	}
}

void  handler_func(int sig)
{
     signal(sig, SIG_IGN);
     printf("got signal");
     signal(sig, handler_func);
}
int main()
{
	int train_fd[3];
	int i,j,k,flag;
							//signals
    int shmid;
    key_t key;
    int pid,available_no;
    int *shm,*s;

	for(i=0;i<3;i++)
	{
		plat_available[i]=1;  //initially all are available
	}

	//arranging for signals
	 pid = getpid();
     key = 5678;
     shmid = shmget(key, 2*sizeof(int), IPC_CREAT | 0666);
     shm = shmat(shmid, NULL, SHM_RND);
     s=shm;
     *s++=pid; *s=-1;
     
    signal(SIGUSR1,handler_func);


    

	for(i=0;i<3;i++)         //opening the file descriptors for trains
	{
		char namebuffer[10];
		sprintf(namebuffer,"%s%d","platform",i+1);
		mkfifo(namebuffer,0666);
		train_fd[i]=open(namebuffer,O_RDONLY|O_NONBLOCK);
		error_func(train_fd[i],"opening the fifo file");
	}	
	
	struct pollfd fd_train[3];   //poll onn train fd's
	for(i=0;i<3;i++)
	{
		fd_train[i].fd=train_fd[i];
		fd_train[i].events=POLLRDNORM;
	}

	while(1)
	{
		error_func(poll(fd_train,3,100),"error while polling\n");
		for(i=0;i<3;i++)
		{
			if(POLLRDNORM & fd_train[i].revents)
			{
				char buffer[15];
				read(train_fd[i],buffer,sizeof(buffer));
				flag=0;
				for(j=0;j<3;j++)
				{
					if(plat_available[j]==1)
					{
						flag=1;
						break;
					}
				}

				if(flag)  //j platform is available
				{
					char trainnamebuffer[15];
					sprintf(trainnamebuffer,"%s%d","train",i+1);
					broadcast_plat(trainnamebuffer);   //broadcast to all platforms arrvial of train i

					char platnamebuffer[15];
					sprintf(platnamebuffer,"%s%d","plat",j+1);

					int train_acceptfd=open(trainnamebuffer,O_WRONLY);
					write(train_acceptfd,platnamebuffer,sizeof(platnamebuffer)); //telling train which platform is available
				}
				else
				{
					//no platfrom is available
				}
			}
		}
	}
}	