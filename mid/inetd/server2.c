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
	printf("%s\n","server2 got forked");
	while(1)
	{
				sleep(3);

		fgets(buffer,sizeof(buffer),stdin);
		printf("%s\n",buffer);
		//sleep(3);
		write(1,"from server2 to client",30);
	}
	return 0;
}