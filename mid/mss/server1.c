#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define max_clients 3

pthread_t threads[max_clients];

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		//exit(1);
	}
}

void *func(void * arg)
{
    int nfd = *((int *)arg);
    printf("%s\n","thread started");

    char namebuffer[30];
    while(1)
    {
    	recv(nfd,namebuffer,sizeof(namebuffer),0);
    	printf("%s%s\n","got from client",namebuffer);
    	send(nfd,"sending from server1",30,0);
    }
}



int main()
{
	int nfd;
	struct sockaddr_in serv_addr,client_addr;
	socklen_t len=sizeof(client_addr);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8000);
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	nfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(nfd,"socket call");

	
	error_func(connect(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect call");
	perror("connect :");
	send(nfd,"8001",10,0);

	//getsockname

	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];

	socklen_t len1=sizeof(addr);
	getsockname(nfd,(struct sockaddr*)&addr,&len1);
	struct sockaddr_in *s = (struct sockaddr_in *)&addr;
	int port=ntohs(s->sin_port);
	inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof (ipstr));
	printf("%s%d\n","The port is",ntohs(s->sin_port));
	printf("Peer IP address: %s\n", ipstr);

	//

	nfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(nfd,"socket call");
	serv_addr.sin_port=htons(8001);
	int set = setsockopt(nfd, SOL_SOCKET, SO_REUSEPORT, &(int){ 1 }, sizeof(int));
		error_func(set,"calling sockpot");

	error_func(bind(nfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(nfd,5),"listen error");

	int noofclients=0;
	while(1)
	{
		int nsfd=accept(nfd,(struct sockaddr*)&client_addr,&len);
		error_func(nsfd,"accept call");
		perror("accept:");
		pthread_create(&threads[noofclients++],NULL,func,(void *)(&nsfd));
	}

	int i;
	for(i=0;i<noofclients;i++)
	{
		pthread_join(&threads[i],NULL);
	}


}