#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

pthread_t thread;

void *thread_func(void* arg)
{
	printf("%s\n","created");
	int nfd=*((int*)arg);
	while(1)
	{
		char buffer[30];
		recv(nfd,buffer,30,0);
		printf("%s\n",buffer);
		sleep(2);
		send(nfd,"hello from server",30,0);
	}

}

void error_func(int val,char* message)
{
	if(val<0)
		perror(message);
}

int main()
{
	int sfd,nfd;
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);
	
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"socket");
	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(sfd,5),"listen");
	struct pollfd fdp[5];
	fdp[0].fd=sfd;
	fdp[0].events=POLLRDNORM;
	int client_count=1;
	while(1)
	{
		error_func(poll(fdp,5,0),"poll");
		if(fdp[0].fd & fdp[0].revents)
		{
			//waiting for input connections
			nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
			error_func(nfd,"accept");
			fdp[client_count].fd=nfd;
			fdp[client_count++].events=POLLRDNORM;
		}
		else
		{
			
		}
		
	}

}