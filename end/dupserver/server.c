#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>

void error_func(int val,char *message)
{
	if(val<0)
		perror(message);
}

int main()
{
	int sfd;
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);

	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"socket");
	int val = 1;
    int result=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR, &val,sizeof(val));  if(result<0) printf("%s\n","sock\n");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(sfd,4),"listen");

	while(1)
	{
		int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		int f=fork();
		if(f==0)
		{
			//child process
			dup2(nfd,0);
			dup2(nfd,1);
			execve("server1",NULL,NULL);
		}
		else
		{
			continue;
		}
	}
	

}