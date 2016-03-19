#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 5

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		//exit(1);
	}
}

pthread_t threads[MAX_CLIENTS];

void * thread_func(void *arg)
{
	int nfd=*((int*)arg);
	printf("%s%d\n","The nfd: ",nfd);

	while(1)
	{
		char buffer[30];
		recv(nfd,buffer,sizeof(buffer),0);
		printf("server :%s\n",buffer);
		sleep(4);
		send(nfd,"from server:",30,0);
	}
}
int main()
{
	int sfd,nfd;
	struct sockaddr_in serv_addr,client_addr;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	socklen_t len=sizeof(client_addr);
	int i,j;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"sock error");

	int set=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&(int){ 1 }, sizeof(int));
	error_func(set,"opt error");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");

	int clients_count=0;
	while(1)
	{
		int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept error");
		pthread_create(&threads[clients_count++],NULL,thread_func,(void *)&nfd);
	}

	for(i=0;i<clients_count;i++)
	{
		pthread_join(&threads[i],NULL);
	}
}