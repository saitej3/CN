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
struct sockaddr_in client_addr;
socklen_t len=sizeof(client_addr);

void * thread_func(void *arg)
{
	int sfd=*((int *)arg);
	printf("%d\n",sfd);
	int nfd;

	nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept error");

	while(1)
	{
		char buf[30];
		recv(nfd,buf,30,0);
		sleep(3);
		send(nfd,"from server:",30,0);
	}
}
int main()
{
	int sfd,nfd;
	struct sockaddr_in serv_addr;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int i,j;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"sock error");

	int set=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&(int){ 1 }, sizeof(int));
	error_func(set,"opt error");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");

	int n=5;
	for(i=0;i<MAX_CLIENTS;i++)
	{
		pthread_create(&threads[i],NULL,thread_func,(void *)&sfd);
	}
	sched_yield();

	for(i=0;i<MAX_CLIENTS;i++)
	{
		pthread_join(&threads[i],NULL);
	}
}