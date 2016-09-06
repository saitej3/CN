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
#include <sys/un.h>


void error_func(int val,char *message)
{
	if(val<0)
		perror(message);
}


int main(int argc ,char* argv[])
{
	int sets,actors;
	while(1)
	{
		printf("%s\n","Enter no of actors and sets needed");
		scanf("%d%d",&actors,&sets);
		int actor_list[actors];
		int sets_list[sets];

		printf("%s\n","Enter actors needed");
		int i;
		for(i=0;i<sets;i++)
		{
			scanf("%d",&sets_list[i]);
		}
		printf("%s\n","Enter sets needed");
		for(i=0;i<actors;i++)
		{
			scanf("%d",&actor_list[i]);
		}
		int nfd;
		struct sockaddr_in serv_addr;
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_port=htons(4002);
		serv_addr.sin_addr.s_addr=INADDR_ANY;

		nfd=socket(AF_INET,SOCK_STREAM,0);
		error_func(nfd,"socket");
		error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"accept");

		char countbuffer[30];
		sprintf(countbuffer,"%d",sets);
		send(nfd,countbuffer,30,0);

		sprintf(countbuffer,"%d",actors);
		send(nfd,countbuffer,30,0);

		for(i=0;i<sets;i++)
		{
			sprintf(countbuffer,"%d",sets_list[i]);
			send(nfd,countbuffer,30,0);
		}

		for(i=0;i<actors;i++)
		{
			sprintf(countbuffer,"%d",actor_list[i]);
			send(nfd,countbuffer,30,0);
		}

	}
}