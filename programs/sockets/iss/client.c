#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(int val,const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char const *argv[])
{
	int sockfd,portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buff[100];
	portno=atoi(argv[2]);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	error(sockfd,"creating socket");

	server=gethostbyname(argv[1]);

	//empty the address
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port=htons(portno);
	bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
	int n;
	error(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"error connecting");
	
	while(1)
	{
		bzero(buff,sizeof(buff));
				fgets(buff,sizeof(buff),stdin);
		n=write(sockfd,buff,strlen(buff));
		error(n,"unable to write");
		n = read(sockfd,buff,sizeof(buff));
		error(n,"unable to read");	
	}
	close(sockfd);
	return 0;

}