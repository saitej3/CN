#include "vivek.h"
#include <pthread.h>

int fd;
int writefd;
pthread_t writethread,readthread;
void *temp;
int gotserver=0;

struct logfds{
	int number;
	int fd[10];
};

void * thread_read(void *arg){
	char buf[100];
	while(1){
		if(read(fd,buf,100) > 0)
		{
				printf("User: ");
				printf("%s\n",buf);
		}
	}
}

int main(int argc, char *argv[])
{
	char buf[100];
	char buf2[100];
	snprintf(buf2,sizeof(buf2),"%s%d","serverfifo",atoi(argv[1]));
	printf("%s\n",buf2);
	//Main Server opening	
	fd=open(buf2,O_RDWR);
	writefd=open(buf2,O_RDWR);
	printf("Opened the fifos....servers ready\n");
	// pthread_create(&writethread,NULL,thread_write,NULL);
	pthread_create(&readthread,NULL,thread_read,NULL);
	// pthread_join(writethread,&temp);
	pthread_join(readthread,&temp);
	close(fd);
	close(writefd);

}
