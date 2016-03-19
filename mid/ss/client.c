#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		exit(1);
	}
}

int main(int argc,char *argv[])
{
	int service_wanted=atoi(argv[1]);
	printf("%s%d\n","service wanted",service_wanted);
	int port=9000+service_wanted-1;

	struct sockaddr_in serv_addr;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=port;
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	int nfd;
	nfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(nfd,"socket call error");

	error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect call error");
	sleep(3);
	send(nfd,"cool",5,0);
	return 0;
}