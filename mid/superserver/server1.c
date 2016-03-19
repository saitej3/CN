#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	char buffer[30];
	printf("%s\n","server1 got forked");
	//scanf("%s",buffer);
	recv(0,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	send(1,"from server1 to client",60,0);
	return 0;
}