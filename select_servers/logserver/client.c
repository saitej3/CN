#include "vivek.h"
#include <pthread.h>

int fd;
int writefd;
pthread_t writethread,readthread;
void *temp;

void * thread_write(void *arg){
	char message[100];
	printf("\nKeep typing message\n");
	while(1)
	{
		printf("User: ");
		fgets(message,100,stdin);
		printf("%s\n",message);
		write(fd,message,sizeof(message));
	}
}

void * thread_read(void *arg){
	char buf[100];
	while(1){
		if(read(writefd,buf,100) > 0)
		{
			printf("Server: ");
			printf("%s\n",buf);
		}
		fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	if(mkfifo(argv[1],0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	char buf[50];
	snprintf(buf, sizeof(buf), "%s%s", argv[1], "write");
	if(mkfifo(buf,0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	fd=open(argv[1],O_RDWR);
	writefd=open(buf,O_RDWR);
	pthread_create(&writethread,NULL,thread_write,NULL);
	// pthread_create(&readthread,NULL,thread_read,NULL);
	pthread_join(writethread,&temp);
	// pthread_join(readthread,&temp);
	close(fd);
	close(writefd);

}
