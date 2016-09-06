#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s\n",message);
		exit(1);
	}
}

int main()
{
	int sfd;
	struct sockaddr_un serv_addr;
	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"main_echo_socket");

	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");

	error_func(connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect error");
	char buffer[30];
	send(sfd,"From client to mainserver",30,0);

	while(1)
	{
		recv(sfd,buffer,30,0);
		printf("client :%s\n",buffer);
		send(sfd,"from client to server",30,0);
	}
	close(sfd);
	return 0;
}