#include "vivek.h"
#include <pthread.h>

int fd;
int writefd;
pthread_t writethread,readthread;
void *temp;
int gotserver=0;
int server_no=0;
struct logfds{
	int number;
	int fd[10];
};
int quit=0;
void * thread_write(void *arg){
	char message[]="server";
	printf("Requesting for a server.............\n");
	printf("%s\n",message);
	write(fd,message,sizeof(message));
	while(1){
		if(gotserver == 1){
				printf("Send a message:\n");
				fgets(message,100,stdin);
				printf("%s\n",message);
				if(strcmp(message,"quit\n") == 0)
				{
					quit=1;
					fd=open("clientfifo",O_RDWR);
					snprintf(message,sizeof(message),"%d",server_no);
					write(fd,message,sizeof(message));
					break;
				}
				write(fd,message,sizeof(message));
		}
	}
}

void * thread_read(void *arg){
	while(quit == 0){
		char buftemp[100];
		if(read(writefd,buftemp,100) > 0)
		{
				printf("Server: ");
				printf("%s\n",buftemp);
				quit=1;
		}
		if(quit == 1){
			printf("Fifo name: %s ",buftemp);
			char *buf;
		    buf = strtok(buftemp, "|");
		    if(buf)
		    {
		    	printf("%s\n",buf);
		    	fd=open(buf,O_RDWR);
		    }
		    buf = strtok(NULL, ",");
		    if(buf)
		    {
		           printf("%s\n", buf);
		           server_no=atoi(buf)+1;
		    }
			char buf2[20];
			snprintf(buf2,sizeof(buf2),"%s%s",buf,"write");
			writefd=open(buf2,O_RDWR);
			gotserver=1;
			fflush(stdout);
		}
	}
}

int main(int argc, char *argv[])
{

	//Main Server opening	
	fd=open("clientfifo",O_RDWR);
	writefd=open("clientfifowrite",O_RDWR);
	pthread_create(&writethread,NULL,thread_write,NULL);
	pthread_create(&readthread,NULL,thread_read,NULL);
	pthread_join(writethread,&temp);
	pthread_join(readthread,&temp);
	close(fd);
	close(writefd);

}
