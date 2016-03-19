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
	int port=atoi(argv[1]);
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	nfd=socket(AF_INET,SOCK_STREAM,0);
	connect(nfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr));

	while(1)
	{
		write(nfd,"from client to server",30);
		char buffer[30];
		read(nfd,buffer,sizeof(buffer));
		printf("%s","client:" );
		printf("%s\n",buffer);
	}
	return 0;
}