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
	int sfd,nsfd;
	struct sockaddr_in server_addr,client_addr;
	socklen_t len;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"error in socket call");
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(8000);
	server_addr.sin_addr.s_addr=INADDR_ANY;

	error_func(bind(sfd,(struct sockaddr *)&server_addr,sizeof(server_addr)),"error in bind");

	error_func(listen(sfd,3),"error in listening");
	nsfd=accept(sfd,(struct sockaddr *) & client_addr,&len);
	error_func(nsfd,"error in accept");
	
	char buffer[30];
	recv(nsfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	sleep(10);
	send(nsfd,"that was awesome from server",30,0);
	return 0;


}