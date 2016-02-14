#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//fdA client to serevr
//fdB server to client
int fdA,fdB;
pthread_t readerthread,writerthread;
void * waiter;
char readbuf[100];
char writebuf[100];

void * writer()
{
	while(1)
	{
		fscanf(stdin,"%s",writebuf);
		write(fdA,writebuf,strlen(writebuf));
		memset(writebuf,0,sizeof(writebuf));
	}
	sleep(1); 

}

void * reader()
{
	while(1)
	{
		int r=read(fdB,readbuf,100);
		if(r>0)
		{
			fprintf(stdout, "%s\n", readbuf);
			memset(readbuf, 0, sizeof(readbuf));
		}
		
	}
	sleep(1);
}

void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s %d\n",message,val);
		exit(1);
	}
}


int main(int argc ,char * argv[])
{
	fdA=open(argv[1],O_WRONLY,0666);
	error_func(fdA,"opening file");
	printf("%s\n",argv[1]);
	printf("done first\n" );

	pthread_create(&readerthread,NULL,reader,NULL);
	pthread_create(&writerthread,NULL,writer,NULL);
	pthread_join(readerthread,NULL);
	pthread_join(writerthread,NULL);	
}	
