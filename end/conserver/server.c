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
#include <poll.h>
#include <sys/un.h>

#define max_clients 5
void error_func(int val,char *message)
{
	if(val<0)
		perror(message);
}

int main()
{
	int sfd;
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);

	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"socket");
	int val = 1;
    int result=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR, &val,sizeof(val));  if(result<0) printf("%s\n","sock\n");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(sfd,4),"listen");
	struct pollfd fdp[max_clients+1]; //fdp[0] for accepting connections
	fdp[0].fd=sfd;
	fdp[0].events=POLLIN;
	int client_count=1;
	printf("%s\n","reached before poll");
	while(1)
	{
		error_func(poll(fdp,max_clients+1,100),"poll");
		printf("%s\n","reached mid poll");
		sleep(2);
		if(fdp[0].fd & fdp[0].revents)
		{
			printf("%s\n","reached");
			int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
			fdp[client_count].fd=nfd;
			fdp[client_count++].events=POLLIN	;
		}
		else
		{
			printf("%s\n","me reached");
			int i;
			for(i=1;i<client_count;i++)
			{
				if(fdp[i].fd & fdp[i].revents)
				{
				    printf("%s\n","me reached main");
					char buffer[30];
					recv(fdp[i].fd,buffer,30,0);
					printf("%s\n",buffer);
					send(fdp[i].fd,"hai from server",30,0);
				}	
			}
		}
	}
	

}