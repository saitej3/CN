#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


void error_func(int val,char *message)
{
	if(val<0)
		perror(message);
}

int main()
{
	int nfd;
	struct sockaddr_un serv_addr;
	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"echo_socket");

	nfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"coonect");

	char buffer[30];
	send(nfd,"hai from client",30,0);
	recv(nfd,buffer,30,0);
	printf("%s\n",buffer);
	return 0;
}