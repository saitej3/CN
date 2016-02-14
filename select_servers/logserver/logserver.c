#include "vivek.h"
#include <sys/select.h>

struct logfds{
	int number;
	int fd[10];
};
//Log Server
int main(int argc,char *argv[]){
	printf("\nLog Server is starting................");
	int fd;
	fd_set readset;
	FD_ZERO(&readset);
	struct timeval timeptr;
	timeptr.tv_sec=0;
	timeptr.tv_usec=0;
	struct logfds logs;
	int i;
	logs.number=atoi(argv[1]);
	for(i=0;i<logs.number;i++){
		char buf[50];
		snprintf(buf, sizeof(buf), "%s%d", "logfifo", i);
		if(mkfifo(buf,0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		if((logs.fd[i]=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
	}
	//Shared memory creation and putting log fd info
	int shmid1;
	key_t key1;
	key1=300;
	int size=10000;
	struct logfds *shm;
	if ((shmid1 = shmget (key1, size, IPC_CREAT | 0666)) == -1) {
	   perror("shmget: shmget failed"); exit(1); } else {
	   (void) fprintf(stderr, "shmget: shmget returned %d\n", shmid1);
	}
	shm = (struct logfds *)shmat(shmid1, NULL, 0);
	shm->number=logs.number;
	for(i=0;i<logs.number;i++)
		shm->fd[i]=logs.fd[i];
	//Creating polling fds
	int max=0;
	for(i=0;i<argc-1;i++)
	{
		FD_SET(logs.fd[i],&readset);
		if(logs.fd[i] > max)
			max=logs.fd[i];
	}
	int temp;
	int j=0;
	int k;
	printf("\nlog fds are created................");
	printf("\nServer started..................");
	while(1){
		int retstatus=select(max+1,&readset,NULL,NULL,&timeptr);
		// printf("Polling\n");
		if(retstatus > 0){
			// printf("Got message\n");
			for(j=0;j<logs.number;j++)
			{
				if(FD_ISSET(logs.fd[j],&readset))
				{
					char buf[100];
					printf("Logged: ");
					if(read(logs.fd[j],buf,100) > 0)
					{
						printf("%s\n",buf);
					}
					fflush(stdout);
				}
			}
		}
		fflush(stdout); 
		for(i=0;i<argc-1;i++)
		{
			FD_SET(logs.fd[i],&readset);
			if(logs.fd[i] > max)
				max=logs.fd[i];
		}
	}
	return 0;
}