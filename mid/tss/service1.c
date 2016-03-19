#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define max_clients 3

pthread_t threads[max_clients];
void error_func(int val, char *message)
{
	if(val<0)
	{
		printf("%s",message);
		exit(1);
	}
}

void *func(void * arg)
{
    int nfd = *((int *)arg);
    printf("%s\n","thread started");

    char namebuffer[30];
    while(1)
    {
    	recv(nfd,namebuffer,sizeof(namebuffer),0);
    	printf("%s%s\n","got from client",namebuffer);
    	send(nfd,"sending from server1",30,0);
    }
}
int main(int argc ,char * argv[])
{
	int port=9000;

	int sfd,nfd;
	struct sockaddr_in serv_addr,client_addr;
	socklen_t len;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"error in socket call");

	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)),"error in bind");
	error_func(listen(sfd,3),"error in listening");

	printf("%s\n","server1 forked");
	int clients_no=0;
	while(1)
	{
		nfd=accept(sfd,(struct sockaddr *) & client_addr,&len);
		error_func(nfd,"error in accept");
		if(clients_no>=2)
		{
			printf("%s\n","max clients reached : Disconnectinf");
			close(nfd);
		}
		else
		{
			printf("%s\n","client accepted");
			pthread_create(&threads[clients_no++],NULL,func,(void *)(&nfd));
		}
	}

	return 0;
}