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
	int usfd;
	struct sockaddr_un serv_addr,client_addr;
	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"echo_socket");
	int len=sizeof(client_addr);
	usfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(usfd,"socket");
	unlink(serv_addr.sun_path);

	error_func(bind(usfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(usfd,4),"listen");
	int nfd=accept(usfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept");

	char buffer[30];
	recv(nfd,buffer,30,0);
	printf("%s\n",buffer);
	send(nfd,"hai from server",30,0);
	return 0;
}