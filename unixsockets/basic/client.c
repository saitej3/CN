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
	strcpy(serv_addr.sun_path,"echo_socket");

	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");

	error_func(connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect error");
	char buffer[30];
	send(sfd,"From client to server",30,0);
	recv(sfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	close(sfd);
	return 0;
}