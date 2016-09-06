#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main()
{
	while(1)
	{
		fflush(stdout);
		char buffer[100];
		bzero(buffer,100);
		scanf("%s",buffer);
		printf("%s\n","service 1 requested and processed");
	}
}