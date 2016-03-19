#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <limits.h>

fd_set readset, testset;; //keyboard,pipe,fifo,popen,//nsfd's
int pfd,ffd,popenfd;
char readbuf[100],writebuf[100];
void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s %d\n",message,val);
		exit(1);
	}
}


void keyboardFunc()
{
	FD_SET(0,&readset);
}

void pipeFunc()
{
	int fd[2];
	error_func(pipe(fd),"Creating the pipe\n");
	int c=fork();
	error_func(c,"forking the process\n");
	if(c==0)
	{
		//child process
		close(fd[0]);
		//write(fd[1],"From Child Pipe",30);
		exit(0);
	}
	else
	{
		//parent process
		close(fd[1]);
    	pfd=fd[0];
    	FD_SET(pfd,&readset);
	}
}

void fifoFunc()
{
	error_func(mkfifo("fifo",0666),"making fifo server\n");
	int fd=open("fifo",O_RDONLY|O_NONBLOCK);
	error_func(fd,"opening file\n");
  ffd=fd;
  FD_SET(ffd,&readset);		
}

void popenFunc()
{
	FILE * f=popen("./popenproc","r");
  popenfd=fileno(f);
  FD_SET(popenfd,&readset);
}

void signalregister()
{
    int shmid;
    key_t key;
    int pid,available_no;
    int *shm,*s;
    pid = getpid();
    key = 5677;
    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shm = shmat(shmid, NULL, SHM_RND);
    s=shm;
    *s=pid;
    signal(SIGUSR1,handler_func);
}


void handler_func(int sig)
{
	//create echo server and send client connection to echoserver

	
	int firstfd=nsfdarr[0];  //afterwards implement queue or heap
	int c=0;
	c=fork();
	if(c==0)
	{
		for(i=1;i<nsfd_count;i++)
		{
			close(nsfdarr[i]);
		}

		close(0);
		close(pfd);
		close(ffd);
		close(popenfd);
		dup(firstfd,0);
		dup(firstfd,1);
		execvp("echoserver",NULL,NULL);
	}
	else
	{

	}



}


int main(int argc, char *argv[])
{

    FD_ZERO(&readset) ;
  	popenFunc();		//incoming connections	
  	keyboardFunc();
  	pipeFunc();t 
  	fifoFunc();
    signalregister();

	struct sockaddr_in sAddr;
    int sfd;
    int nsfd;
    char buffer[25];
    int result;
    int nread;
    int x;
    int val;

    sfd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
    val = 1;
    result = setsockopt(sfd,SOL_SOCKET, SO_REUSEADDR, &val,sizeof(val));
      if (result < 0) {
            perror("server1");
             return 0;
      }

    sAddr.sin_family = AF_INET;
  	sAddr.sin_port = htons(8898);  
  	sAddr.sin_addr.s_addr = INADDR_ANY;
    result = bind(sfd, (struct sockaddr*) &sAddr, sizeof(sAddr));
  	if (result < 0) {
    	perror("server1");
    	return 0;
  	}

   result = listen(sfd, 5);
   if (result < 0) {
     perror("server1");
     return 0;
   }

   FD_SET(sfd, &readset);

    while (1) 
    {

    testset = readset;
    result = select(FD_SETSIZE, &testset,NULL, NULL, NULL);
    if (result < 1) {
      perror("server1");
      return 0;
    }

    for (x = 0; x < FD_SETSIZE; x++) 
    {
        if (FD_ISSET(x, &testset))
        {

      	    if (x == sfd)
      	     {
                 	nsfd = accept(sfd, NULL,NULL);
          			FD_SET(nsfd, &readset);
             }

             else if(x==0 || x==pnrfd ||x==ffd || x==popenfd)
             {
                nread=read(0,buffer,sizeof(buffer));
                if(nread<=0)
                {
                	nread=read(pfd,buffer,sizeof(buffer));
                	if(nread<=0)
                	{
                		nread=read(ffd,buffer,sizeof(buffer));
                		if(nread<=0)
                		{
		                	nread=read(popenfd,buffer,sizeof(buffer));
                		}
                	}
                }

                for(i=0;i<nsfd_count;i++)
                {
                	send(nsfdarr[i],buffer,sizeof(buffer));
                }
                
             }
            else
            {
                  nread = recv(x, buffer, 25, 0);
                  if (nread <= 0)
                  {
                    close(x);
                    FD_CLR(x, &readset);
                    printf("client on descriptor #%i disconnected\n", x);
                  } 
                else
                 {
                    buffer[nread] = '\0';
                    printf("%s\n", buffer);
                    send(x, buffer, nread, 0);
                 }
            }
        }
    }
  }

 
}	