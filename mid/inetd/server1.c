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
	//printf("%s\n","server1 got forked");
	while(1)
	{
	// 	sleep(3);
	 //	fflush(stdin);
		//fgets(buffer,sizeof(buffer),stdin);
		scanf("%s",buffer);
		printf("%s\n",buffer);
		//sleep(3);
	
		write(1,"from server1 to client",30);
	}
	return 0;
}