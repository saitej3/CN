#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc,char *argv[])
{
	int nfd;
	struct sockaddr_in serv_addr;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8002);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	nfd=socket(AF_INET,SOCK_STREAM,0);
	connect(nfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr));

	send(nfd,"from client to server",30,0);
	char buffer[60];
	recv(nfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	return 0;
}