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

void error_func(int val, char *message)
{
	if(val<0)
	{
		perror(message);
		exit(1);
	}
}

int available_count=0;
pthread_t thread;
pthread_t threads[5];

void *thread_func()
{
		printf("%s\n","thread created raw" );

	struct sockaddr_in s_addr, d_addr;
    struct sockaddr_in addr; 
    int rsfd, n, len, val=1;
    char buffer[1024], buf[1024];
    rsfd = socket(AF_INET, SOCK_RAW, 101);
    setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));
    struct iphdr * iph; unsigned int iphdrlen;
   
       while(1)
       {
       	printf("%s\n","came");
       	 len = sizeof(addr);
        int r=recvfrom(rsfd, buffer, 1024, 0, (struct sockaddr *)&addr, &len);
        struct sockaddr_in source,dest;
        unsigned short iphdrlen;     
        struct iphdr *iph = (struct iphdr *)buffer;
        iphdrlen =iph->ihl*4;
         
        strcpy(buf, buffer+iphdrlen);
        printf("Message from producer: %s", buf);
        if(r>1)
        {
        	printf("r value%d\n",r );
        	return ;
        }
       }
}

void * thread_funcmain(void * arg)
{
	int nfd = *((int *)arg);
	printf("available_count%d\n",available_count);
	if(available_count==0 || available_count==2)
		{
			available_count++;
			char permbuffer[30];
			sprintf(permbuffer,"%d",1);
			send(nfd,permbuffer,30,0);
			pthread_create(&thread,NULL,thread_func,NULL);

		}
		else
		{					
			available_count++;
			char permbuffer[30];
			sprintf(permbuffer,"%d",0);
			send(nfd,permbuffer,30,0);
			return ;
		}
		char newbuffer[30];
		read(nfd,newbuffer,30);
		printf("%s\n",newbuffer);
}

int main(int argc,char *argv[])
{
	int sfd,nfd;
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);
	
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"socket");
	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind");
	error_func(listen(sfd,5),"listen");

	int clients_no=0;
	while(1)
	{
		nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept");
		printf("%s\n","connected to set");
		printf("%d\n",nfd);
		pthread_create(&threads[clients_no++],NULL,thread_funcmain,(void *)(&nfd));
	}

}