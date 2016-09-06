#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void error_func(int val,char* message)
{
	if(val<0)
		perror(message);
}

int main()
{
	int nfd;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	nfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect");

	char namebuf[30];
	sprintf(namebuf,"%d",8040);
	send(nfd,namebuf,30,0);
	char buffer[30];
	sprintf(buffer,"%s","sample.txt");
	send(nfd,buffer,30,0);
	close(nfd);
	//create a new socket to accept the connections

	int newsfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in client_addr;
	int len=sizeof(client_addr);
	serv_addr.sin_port=htons(8040);
	error_func(newsfd,"socket");
	int set = setsockopt(newsfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(newsfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(newsfd,5),"listen");
	nfd=accept(newsfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept");
	printf("%s\n","FILE contents:" );
	
	while(1)
	{
		char buf[255];
		recv(nfd,buf,255,0);
		printf("%s\n",buf);
	}
	
}