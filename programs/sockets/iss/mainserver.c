#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
# define SIZE 4
void error(int val,const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
	int sfd[SIZE],nsfd,port[SIZE];
	socklen_t clientn;

	//get port numbers
	int i;
	printf("%s\n","Enter the port numbers");
	for(i=0;i<SIZE;i++)
	{
		scanf("%d",&port[i]);
	}

	struct sockaddr_in serv_addr[SIZE];
	for(i=0)
}