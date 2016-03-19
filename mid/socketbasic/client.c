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

int main()
{
	int sfd;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	send(sfd,"that was aweosome",30,0);
	char buffer[30];
	recv(sfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer );
	return 0;
}