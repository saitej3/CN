#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define max_services 5

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		//exit(1);
	}
}


int main()
{
	int nfd;
	struct sockaddr_in serv_addr;

	serv_addr.sin_port=htons(8000);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	nfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(nfd,"socket error");
	error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect error");
	send(nfd,"The train sequence ---1234",30,0);
	sleep(2); //train is waiting in the station and about to leave...
	send(nfd,"I am leaving",30,0);
	sleep(2);

} 