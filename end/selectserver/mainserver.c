#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#define max_clients 3
void error_func(int val,char * message)
{
	if(val<0)
	{
		perror(message);
	}
}

int fds[max_clients];
fd_set readset;
int max_fd;
void rep()
{
	int i;
	FD_ZERO(&readset);
	for(i=0;i<max_clients;i++)
	{
		FD_SET(fds[i],&readset);
		if(max_fd<fds[i])
			max_fd=fds[i];
	}
}
void handler_func()
{
	printf("%s\n","got signal :)" );
}

int main()
{
	signal(SIGUSR1,handler_func);
	printf("%d\n",getpid());
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);

	int i;
	for(i=0;i<max_clients;i++)
	{
		fds[i]=socket(AF_INET,SOCK_STREAM,0);
		error_func(fds[i],"socket");
		serv_addr.sin_port=htons(8000+i);
		error_func(bind(fds[i],(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
		error_func(listen(fds[i],4),"listen");
	}

	while(1)
	{
		rep();
		error_func(select(max_fd+1,&readset,NULL,NULL,0),"select");
		int i,j;
		for(i=0;i<max_clients;i++)
		{
			if(FD_ISSET(fds[i],&readset))
			{
				int nfd=accept(fds[i],(struct sockaddr*)&client_addr,&len);
				error_func(nfd,"accpet");
				int f=fork();
				if(f==0)
				{
					dup2(nfd,0);
					dup2(nfd,1);
					execve("server1",NULL,NULL);
				}
				else
					continue;
				
			}
		}
	}
}