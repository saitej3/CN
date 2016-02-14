#include "vivek.h"

int fd;
int writefd;
void *temp;
int gotserver=0;
int clientno;
struct logfds{
	int number;
	int fd[10];
};


// void * thread_read(void *arg){
// 	char buf[100];
// 	if(read(writefd,buf,100) > 0)
// 	{
// 			printf("Server: ");
// 			printf("%s\n",buf);
// 	}
// 	printf("Fifo name: %s ",buf);
// 	fd=open(buf,O_RDWR);
// 	char buf2[20];
// 	snprintf(buf2,sizeof(buf2),"%s%s",buf,"write");
// 	writefd=open(buf2,O_RDWR);
// 	gotserver=1;
// 	fflush(stdout);
// 	while(1){
// 		if(read(writefd,buf,100) > 0)
// 		{
// 				printf("Server: ");
// 				printf("%s\n",buf);
// 		}
// 	}
// }

int main(int argc, char *argv[])
{
	char buf[100];
	char buf2[100];
	if(mkfifo("mainfifo",0777) !=0)
	{
		perror ("The following error occurred");
		exit(0);
	}
	// snprintf(buf2,sizeof(buf2),"%s%d","mainfifowrite",logno);
	//Main Server opening	
	fd=open("mainfifo",O_RDWR);
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
