/*Server goes here
 *
 * */

#include "vivek.h"
#include <sys/select.h>

int main(int argc,char *argv[]){
	fd_set readset;
	FD_ZERO(&readset);
	struct timeval timeptr;
	timeptr.tv_sec=0;
	timeptr.tv_usec=0;
	char message[200];
	printf("\nServer is starting................ Clients %d",argc);
	int *fd;
	int *writefd;
	int i;
	fd= malloc((argc-1)*sizeof(int));
	writefd=malloc((argc-1)*sizeof(int));
	for(i=1;i<argc;i++){
		char buf[50];
		if((fd[i-1]=open(argv[i],O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		snprintf(buf, sizeof(buf), "%s%s", argv[i], "write");
		if((writefd[i-1]=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		memset(buf,0,sizeof(buf));

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
			printf("Got message\n");
			for(j=0;j<argc-1;j++)
			{
				if(FD_ISSET(fd[j],&readset))
				{
					char buf[100];
					printf("User %d: ",j);
					if(read(fd[j],buf,100) > 0)
					{
						printf("%s\n",buf);
					}
					snprintf(message,sizeof(message),"User %d: %s",j,buf);
					for(k=0;k<argc-1;k++)
						if(k!=j)
							write(writefd[k],message,sizeof(message));
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
			
		
	
	
