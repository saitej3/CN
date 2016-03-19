#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int sfd,nsfd;
	struct sockaddr_in serv_addr,client_addr;
	socklen_t len;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8002);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	sfd=socket(AF_INET,SOCK_STREAM,0);
	bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	listen(sfd,3);

	fd_set readset,tempset;
	FD_ZERO(&readset);
	FD_SET(sfd,&readset);
	while(1)
	{
		printf("%s\n","camehere");
		tempset=readset;
		select(sfd+1,&tempset,NULL,NULL,0);
		printf("%s\n","camehere1");
		if(FD_ISSET(sfd,&tempset))
		{
			printf("%s\n","camehere2");
			nsfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
			int c=fork();
			if(c==0)
			{
				printf("%s\n","from childprocess");
				close(sfd);
				dup2(nsfd,0);
				dup2(nsfd,1);
				execve("server1",NULL,NULL);
			}
			else
			{
				wait(NULL);
				close(nsfd);
			}
		}
	}
	char buffer[30];
	recv(nsfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);

	send(nsfd,"from server to client",30,0);
	return 0;
}