#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define services_count 3

struct service
{
	char *name;
	int port;
	int sfd;
}services[services_count];

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		exit(1);
	}
}

void server_init()
{
	int i;
	for(i=0;i<services_count;i++)
	{
		services[i].port=9000+i;
		char servnamebuffer[30];
		sprintf(servnamebuffer,"%s%d","server",i+1);
		services[i].name=servnamebuffer;
	}

}

//global variables	
int fdp[services_count];
fd_set readset,tempset;


void rep()
{
	printf("%s\n","called");
	FD_ZERO(&readset);
	int i;
	for(i=0;i<services_count;i++)
	{
		printf("%d",fdp[i]);
	}
	for(i=0;i<services_count;i++)
	{
		FD_SET(fdp[i],&readset);
	}
}

int main()
{
	server_init();

	int maxfd=-1;

	struct sockaddr_in  serv_addr,client_addr;
	socklen_t len;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int i;

	for(i=0;i<services_count;i++)
	{
		services[i].sfd=socket(AF_INET,SOCK_STREAM,0);
		fdp[i]=services[i].sfd;

		if(maxfd<services[i].sfd) maxfd=services[i].sfd;

		int set = setsockopt(services[i].sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
		error_func(set,"calling sockpot");
		error_func(services[i].sfd,"socket call error");
		serv_addr.sin_port=services[i].port;
		error_func(bind(services[i].sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind call error");
		error_func(listen(services[i].sfd,3),"listen call error");
	}

	printf("%s%d\n","max fd is",maxfd);

	while(1)
	{
		printf("%s\n","entered");
		rep();
		error_func(select(maxfd+1,&readset,NULL,NULL,0),"select call error");
		int i,j;
		for(i=0;i<=maxfd;i++)
		{
			for(j=0;j<services_count;j++)
			{
				printf("%s\n","loop construct");
				if(FD_ISSET(services[j].sfd,&readset))
				{
					printf("%s%d\n","the velue of j: ",j);
					int nfd=accept(services[j].sfd,(struct sockaddr*)&client_addr,&len);
					printf("%s%d\n","accpted client : ",nfd);
					char buffer[5];
					recv(services[j].sfd,buffer,5,0);
					printf("%s%s\n","server :",buffer);
					printf("%s%s\n",services[j].name,"got selected");
					FD_CLR(services[j].sfd,&readset);
					printf("%s\n","reached me");
					sleep(2);
				}
			}
		}
	}



}