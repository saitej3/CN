#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>


void error_func(int val,char *message)
{
	if(val<0)
		perror(message);
}

int main()
{
	int usfd;
	struct sockaddr_in serv_addr,client_addr;
	int len=sizeof(client_addr);
	serv_addr.sin_family=PF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	usfd=socket(PF_INET,SOCK_DGRAM,0);
	error_func(usfd,"socket");

	char buffer[30];
    sendto(usfd,"hai from client",30,0,(struct sockaddr*)&serv_addr,len);
	recvfrom(usfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,&len);
	printf("%s\n",buffer);
	printf("%d\n",client_addr.sin_addr.s_addr);
	return 0;
}