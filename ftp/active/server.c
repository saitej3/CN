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
	//waiting for input connections on port 21

	nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept");
	char buffer[30];
	recv(nfd,buffer,30,0); //receive port to connect to
	int port=atoi(buffer);
	printf("PORT:%d\n",port);
	recv(nfd,buffer,30,0); //receive filename
	printf("File Name:%s\n",buffer);
	close(nfd);

	//connect to new port from port 20    //how to bind the client to 20
	int newfd=socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_port=htons(port);
	error_func(connect(newfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect");
	FILE *fp;
   	fp = fopen(buffer, "r+");
   	char buf[255];
   	while(!feof(fp))
   	{
   		fscanf(fp,"%s",buf);
   		sleep(1);
   		send(newfd,buf,255,0);
   	}
}