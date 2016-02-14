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

int status[10];
int count=0;
int start=0;


void * thread_write(void *arg){
	char message[100];
	while(1){
		if(gotserver == 1){
			gotserver=0;
			int j;
			for(j=0;j<count;j++)
				if(status[j] == 0)
				{
					start=j;
					break;
				}
			snprintf(message,sizeof(message),"%s%d|%d","serverfifo",start,start);
			printf("%s : Writing\n",message);
			status[start]=1;
			write(writefd,message,sizeof(message));
		}
 	}

}

void * thread_read(void *arg){
	char buf[100];
	int j;
	while(1){
		if(read(fd,buf,100) > 0)
		{
				printf("%s: Read\nHello",buf);
				if(strcmp(buf,"server") == 0)
					gotserver=1;
				else if((j=atoi(buf)!=0))
				{
					status[j-1]=0;
					printf("Status of %d is changed back to 0\n",j-1);
				}
		}
	}
	
}

//Log Server
int main(int argc,char *argv[]){
	printf("\nMain Server is starting................");
	struct logfds logs;
	struct logfds logwrite;
	int i;
	logs.number=atoi(argv[1]);
	logwrite.number=logs.number;
	for(i=0;i<logs.number;i++){
		char buf[50];
		char buf2[50];
		snprintf(buf, sizeof(buf), "%s%d", "serverfifo", i);
		if(mkfifo(buf,0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		// snprintf(buf2, sizeof(buf2), "%s%d", "mainfifowrite", i);
		// if(mkfifo(buf2,0777) !=0)
		// {
		// 	perror ("The following error occurred");
		// 	exit(0);
		// }
		if((logs.fd[i]=open(buf,O_RDWR)) < 0)
		{
			perror ("The following error occurred");
			exit(0);
		}
		// if((logwrite.fd[i]=open(buf2,O_RDWR)) < 0)
		// {
		// 	perror ("The following error occurred");
		// 	exit(0);
		// }
		status[i]=0;
		count++;
	}

	if(mkfifo("clientfifo",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	if(mkfifo("clientfifowrite",0777) !=0)
		{
			perror ("The following error occurred");
			exit(0);
	}
	fd=open("clientfifo",O_RDWR);
	writefd=open("clientfifowrite",O_RDWR);
	pthread_create(&writethread,NULL,thread_write,NULL);
	pthread_create(&readthread,NULL,thread_read,NULL);
	pthread_join(writethread,&temp);
	pthread_join(readthread,&temp);
	close(fd);
	close(writefd);
	
}