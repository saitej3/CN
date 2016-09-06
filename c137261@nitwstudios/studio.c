#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <sys/un.h>

void error_func(int val, char *message)
{
	if(val<0)
	{
		perror(message);
		exit(1);
	}
}

struct service
{
	int port;
}services[4];

void server_init()
{
	int i;
	for(i=0;i<4;i++)
	{
		services[i].port=9000+i;
	}
}

int main()
{
	server_init();
	int sfd,nfd;
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(5000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);
	
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"socket");
	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(sfd,5),"listen");

	while(1)
	{
		nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept");
		printf("%s\n","connected to studio");
		char buffer[30];
		recv(nfd,buffer,30,0);
		int setcount=atoi(buffer);
		int i;
		for(i=0;i<setcount;i++)
		{
			recv(nfd,buffer,30,0);
			int index=atoi(buffer);
			printf("%d\n",index);
			char portbuffer[30];
			sprintf(portbuffer,"%d",services[index-1].port);
			send(nfd,portbuffer,30,0);
		} 
	}
}