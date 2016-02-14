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

void * thread_write(void *arg){
	char message[100];
	printf("Which server S1 or S2:\n");
	fgets(message,100,stdin);
	printf("%s\n",message);
	write(fd,message,sizeof(message));
	while(1){
		if(gotserver == 1){
				printf("Send a message:\n");
				fgets(message,100,stdin);
				printf("%s\n",message);
				write(fd,message,sizeof(message));
		}
	}
}

void * thread_read(void *arg){
	char buf[100];
	if(read(writefd,buf,100) > 0)
	{
			printf("Server: ");
			printf("%s\n",buf);
	}
	printf("Fifo name: %s ",buf);
	fd=open(buf,O_RDWR);
	char buf2[20];
	snprintf(buf2,sizeof(buf2),"%s%s",buf,"write");
	writefd=open(buf2,O_RDWR);
	gotserver=1;
	fflush(stdout);
	while(1){
		if(read(writefd,buf,100) > 0)
		{
				printf("Server: ");
				printf("%s\n",buf);
		}
	}
}

int main(int argc, char *argv[])
{
	char buf[100];
	char buf2[100];
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
	snprintf(buf,sizeof(buf),"%s%d","mainfifo",logno);
	snprintf(buf2,sizeof(buf2),"%s%d","mainfifowrite",logno);

	//Main Server opening	
	fd=open(buf,O_RDWR);
	writefd=open(buf2,O_RDWR);
	pthread_create(&writethread,NULL,thread_write,NULL);
	pthread_create(&readthread,NULL,thread_read,NULL);
	pthread_join(writethread,&temp);
	pthread_join(readthread,&temp);
	close(fd);
	close(writefd);

}
