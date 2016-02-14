/*Server goes here
 *
 * */

#include "vivek.h"
 #include <sys/select.h>

struct logfds{
	int number;
	int fd[10];
};

int main(int argc,char *argv[]){
	fd_set readset;
	FD_ZERO(&readset);
	struct timeval timeptr;
	timeptr.tv_sec=0;
	timeptr.tv_usec=0;
	printf("\nServer is starting................");
	int *fd;
	int logfd;
	int i;
	char buf[50];
	fd= malloc((argc-1)*sizeof(int));
	for(i=1;i<argc;i++){
		if((fd[i-1]=open(argv[i],O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
	}

	//Getting logfifo number from shared memory
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
	printf("Current Log fd number:%d \n",shm->number);
	shm->number=shm->number-1;
	int logno=shm->number;
	snprintf(buf,sizeof(buf),"%s%d","logfifo",logno);

	//Log Server opening
	if((logfd=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}	
	int max=0;
	for(i=0;i<argc-1;i++)
	{
		FD_SET(fd[i],&readset);
		if(fd[i] > max)
			max=fd[i];
	}
	int temp;
	int j=0;
	int k;
	printf("\nPoll fds are created................");
	printf("\nServer started..................");
	while(1){
		int retstatus=select(max+1,&readset,NULL,NULL,&timeptr);
		if (retstatus <0)
			perror("Error: ");
		// printf("Polling %d\n",retstatus);
		if(retstatus > 0){
			// printf("Got message\n");
			for(j=0;j<argc-1;j++)
			{
				if(FD_ISSET(fd[j],&readset)){
					char buf[100];
					printf("User %d: ",j);
					if(read(fd[j],buf,100) > 0)
					{
						printf("%s\n",buf);
					}
					write(logfd,buf,sizeof(buf));
					fflush(stdout);
				}
			}
		}
		fflush(stdout); 
		for(i=0;i<argc-1;i++)
		{
			FD_SET(fd[i],&readset);
			if(fd[i] > max)
				max=fd[i];
		}
	}
	return 0;
}
			
		
	
	
