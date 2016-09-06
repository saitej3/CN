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
	int nfd;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	nfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(nfd,"socket");
	error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"accept");
	char buffer[30];
	send(nfd,"iahfrom client",30,0);
	recv(nfd,buffer,30,0);
	printf("%s\n",buffer);
}