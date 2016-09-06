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

	char portbuf[30];
	recv(nfd,portbuf,30,0);

	int port=atoi(portbuf);
	printf("PORT:%d\n",port);

	char buffer[30];
	sprintf(buffer,"%s","sample.txt");
	send(nfd,buffer,30,0);
	close(nfd);
	//create a new socket to accept the connections
	sleep(2);
	serv_addr.sin_port=htons(port);
	int newnfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(connect(newnfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect");
	while(1)
	{
		char buf[255];
		recv(newnfd,buf,255,0);
		printf("%s\n",buf);
	}
	
}