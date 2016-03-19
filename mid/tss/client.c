#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void error_func(int val, char *message)
{
	if(val<0)
	{
		printf("%s",message);
	}
}


int main(int argc,char*argv[])
{
	int sfd;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	sfd=socket(AF_INET,SOCK_STREAM,0);
	connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	send(sfd,argv[1],sizeof(argv[1]),0);
	char portbuffer[10];
	recv(sfd,portbuffer,sizeof(portbuffer),0);
	int port=atoi(portbuffer);
	printf("%d\n",port);

	//sleep(5);
	int csfd=socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_port=htons(port);
	connect(csfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	while(1)
	{
		printf("%s\n","connection9" );
		error_func(send(csfd,"from client to server",30,0),"sending error");
				printf("%s\n","connection3" );

		char printbuffer[30];
		sleep(3);
				printf("%s\n","connection4" );

		error_func(recv(csfd,printbuffer,sizeof(printbuffer),0),"recv error");
				printf("%s\n","connection5" );

		printf("%s\n",printbuffer);
		printf("%s\n","connection2" );
	}
}