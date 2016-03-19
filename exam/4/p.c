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

int main(int argc,char* argv[])
{
	int fd_broadcast;
	int train_info;
	char broadnamebuffer[15],infonamebuffer[15];
	sprintf(broadnamebuffer,"%s%s","platbrod",argv[1]);
	sprintf(infonamebuffer,"%s%s","platinfo",argv[1]);

	//arranging to send the signal

	 int shmid;
     key_t key;
     int *shm, *s;

     key = 5678;
     shmid = shmget(key, 2*sizeof(int), 0666);
     shm = (int*) shmat(shmid, NULL, SHM_RND);
     s=shm;
     printf("the pid is %d\n",*s++);
     printf("the no is %d\n",*s);


	mkfifo(broadnamebuffer,0666);
	mkfifo(infonamebuffer,0666);
	fd_broadcast=open(broadnamebuffer,O_RDONLY|O_NONBLOCK);
	train_info=open(infonamebuffer,O_RDONLY|O_NONBLOCK);

	struct pollfd fdp[2];

	fdp[0].fd=fd_broadcast;
	fdp[0].events=POLLRDNORM;

	fdp[1].fd=train_info;
	fdp[1].events=POLLRDNORM;

	while(1)
	{
		error_func(poll(fdp,2,0),"error in polling");

		if(POLLRDNORM & fdp[0].revents)
		{
			//broadcast info
			char broadbuffer[15];
			read(fd_broadcast,broadbuffer,sizeof(broadbuffer));
			printf("%s\n",broadbuffer);
		}

		else if(POLLRDNORM & fdp[1].revents)
		{
			//train information info

			printf("%d\n",fdp[1].revents);
			char infobuffer[30];
			read(train_info,infobuffer,sizeof(infobuffer));
			if(strcmp(infobuffer,"sequence")==0)
			{
				printf("%s\n","got sequence");
			}
			else
			{
				printf("%s\n","train is leaving");
				 //send signal
			}
		}

		else
		{
			continue;
		}
	}
}