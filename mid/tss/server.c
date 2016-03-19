#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

struct service{
	char *name;
	int port;
	int started;
}services[3];

void error_func(int val, char *message)
{
	if(val<0)
	{
		printf("%s",message);
		exit(1);
	}
}

void server_init()
{
	int i;
	for(i=0;i<3;i++)
	{
		services[i].port=9000+i;
		char servnamebuffer[20];
		sprintf(servnamebuffer,"%s%d","service",i+1);
		services[i].name=servnamebuffer;
		services[i].started=0;
	}
}

int main()
{
	server_init();
	int sfd,nsfd;
	struct sockaddr_in server_addr,client_addr;
	socklen_t len;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"error in socket call");
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(8000);
	server_addr.sin_addr.s_addr=INADDR_ANY;

	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");

	error_func(bind(sfd,(struct sockaddr *)&server_addr,sizeof(server_addr)),"error in bind");

	error_func(listen(sfd,3),"error in listening");

	fd_set readset,tempset;
	FD_ZERO(&readset);
	int max_fd=sfd;
	FD_SET(sfd,&readset);

	while(1)
	{
		tempset=readset;
		error_func(select(max_fd+1,&tempset,NULL,NULL,0),"error in select");
		int i;
		for(i=0;i<=max_fd;i++)
		{
			if(FD_ISSET(sfd,&tempset))
			{
				//got a client on sfd
				nsfd=accept(sfd,(struct sockaddr *) & client_addr,&len);
				error_func(nsfd,"error in accept");

				char servbufer[30];
				recv(nsfd,servbufer,sizeof(servbufer),0);
				int service_index=atoi(servbufer);
				printf("%s%d\n","The service requested is",service_index);
				if(services[service_index].started==0)
				{
					//server is not started
					char servnamebuffer[30];
					sprintf(servnamebuffer,"%s%d","service",service_index+1);
					printf("%s%s\n","servname: ",servnamebuffer);
					int c=fork();
					if(c==0)
					{
						printf("%s\n","forked child");
						execve(servnamebuffer,NULL,(void *)0);
					}
					else
					{
						//wait(NULL);
						sleep(5);
						services[service_index].started=1;
					}
				}

				printf("%s\n","reached");

				//service will handle directly
				char writebuf[30];
				sprintf(writebuf,"%d",services[service_index].port);
				send(nsfd,writebuf,sizeof(writebuf),0);
				
				printf("%s\n","reached1");

			}
		}
	}


}