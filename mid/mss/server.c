#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define max_services 5

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		//exit(1);
	}
}
//global variables

int fdp[2];
fd_set readset;
int max_fd;

struct service
{
	int port;
	int sfd;
	int available;
}services[max_services];

void rep()
{
	FD_ZERO(&readset);
	FD_SET(fdp[0],&readset);
	FD_SET(fdp[1],&readset);
	if(fdp[0]>fdp[1])
		max_fd=fdp[0];
	else
		max_fd=fdp[1];
}

void server_init()
{
	int i;
	for(i=0;i<max_services;i++)
	{
		services[i].available=0;
	}
}

int main()
{
	int csfd,ssfd;
	struct sockaddr_in servc_addr,client_addr,servs_addr;
	socklen_t len=sizeof(client_addr);
	int i,j;

	//init
	servs_addr.sin_family=AF_INET;
	servs_addr.sin_port=htons(8000);
	servs_addr.sin_addr.s_addr=INADDR_ANY;

	servc_addr.sin_family=AF_INET;
	servc_addr.sin_port=htons(9000);
	servc_addr.sin_addr.s_addr=INADDR_ANY;

	ssfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(ssfd,"socket call error");
	fdp[0]=ssfd;

	csfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(csfd,"socket call error");
	fdp[1]=csfd;

	printf("%d\n",ssfd);printf("%d\n",csfd);

	int set = setsockopt(ssfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
		error_func(set,"calling sockpot");
	  set = setsockopt(csfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
		error_func(set,"calling sockpot");
			
	error_func(bind(ssfd,(struct sockaddr*)&servs_addr,sizeof(servs_addr)),"bind error");
	error_func(bind(csfd,(struct sockaddr*)&servc_addr,sizeof(servc_addr)),"bind error1");
		perror("bind :");

	error_func(listen(ssfd,5),"listen error");
	error_func(listen(csfd,3),"listen error");



	int no_of_services=0;
	while(1)
	{
		rep();
		printf("%s%d\n","max fd: ",max_fd);
		error_func(select(max_fd+1,&readset,NULL,NULL,NULL),"select call error");
		int i,j;
		for(i=0;i<=max_fd;i++)
		{
			if(FD_ISSET(ssfd,&readset))
			{
				int nfd=accept(ssfd,(struct sockaddr*)&client_addr,&len);
				perror("accept:");

				printf("%d\n",(int) ntohs(client_addr.sin_port)); 

				//get sock name;
				struct sockaddr_storage addr;
				char ipstr[INET6_ADDRSTRLEN];
				socklen_t len1=sizeof(addr);
				getpeername(nfd,(struct sockaddr*)&addr,&len1);
				struct sockaddr_in *s = (struct sockaddr_in *)&addr;
				int port=ntohs(s->sin_port);
				inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
				printf("%s%d\n","The port is",ntohs(s->sin_port));
				printf("Peer IP address: %s\n", ipstr);

				error_func(nfd,"accept error");
				services[no_of_services].sfd=nfd;
				services[no_of_services].available=1;

				char portbuffer[10];
				recv(nfd,portbuffer,sizeof(portbuffer),0);
				port=atoi(portbuffer);
				
				//get peername connection
				// struct sockaddr_storage addr;
				// getpeername(nfd, (struct sockaddr*)&addr, &len);
				// struct sockaddr_in *s = (struct sockaddr_in *)&addr;

				// int port=ntohs(s->sin_port);


				printf("%s%d\n","connected port :",port);
				services[no_of_services++].port=port;
				FD_CLR(ssfd,&readset);
			}
			else if(FD_ISSET(csfd,&readset))
			{
				int nfd=accept(csfd,(struct sockaddr*)&client_addr,&len);
				error_func(nfd,"accept error");
				perror("accept:");

				char servbuffer[10];
				memset(servbuffer,0,sizeof(servbuffer));
				recv(nfd,servbuffer,sizeof(servbuffer),0);
				int serv_index=atoi(servbuffer);
				printf("%s%d\n","service requested is",serv_index);

				if(services[serv_index-1].available==0)
				{
					printf("%s\n","service not available");
					close(nfd);
				}
				else
				{
					printf("%d\n",services[serv_index-1].port);
					printf("%s\n","service is available");
					char servportbuffer[10];
					memset(servportbuffer,0,sizeof(servportbuffer));
					sprintf(servportbuffer,"%d",services[serv_index-1].port);
					printf("%s\n",servportbuffer);
					send(nfd,servportbuffer,sizeof(servportbuffer),0);
					close(nfd);
				}
			}
			else
			{
				continue;
			}
		}
	}

}