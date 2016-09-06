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
#include <netdb.h>
#include <netinet/if_ether.h>

#define max_count 5
void error_func(int val, char *message)
{
	if(val<0)
	{
		perror(message);
		exit(1);
	}
}

struct service
{
	int port;
	int sfd;
	int available;
	int type;
}services[3];

static int send_file_descriptor(int socket, int fd_to_send)
{
 struct msghdr message;
 struct iovec iov[1];
 struct cmsghdr *control_message = NULL;
 char ctrl_buf[CMSG_SPACE(sizeof(int))];
 char data[1];

 memset(&message, 0, sizeof(struct msghdr));
 memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* We are passing at least one byte of data so that recvmsg() will not return 0 */
 data[0] = ' ';
 iov[0].iov_base = data;
 iov[0].iov_len = sizeof(data);

 message.msg_name = NULL;
 message.msg_namelen = 0;
 message.msg_iov = iov;
 message.msg_iovlen = 1;
 message.msg_controllen =  CMSG_SPACE(sizeof(int));
 message.msg_control = ctrl_buf;

 control_message = CMSG_FIRSTHDR(&message);
 control_message->cmsg_level = SOL_SOCKET;
 control_message->cmsg_type = SCM_RIGHTS;
 control_message->cmsg_len = CMSG_LEN(sizeof(int));

 *((int *) CMSG_DATA(control_message)) = fd_to_send;

 return sendmsg(socket, &message, 0);
}

int main()
{
	int actors[max_count];
	int sets[max_count];
	int actors_accp[max_count];
	int sets_accp[max_count];

	int sfd,nfd;
	struct sockaddr_in serv_addr,client_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(4002);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	int len=sizeof(client_addr);
	
	sfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(sfd,"socket");
	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bindme");
	error_func(listen(sfd,5),"listen");

	int s_a=0;
	int a_a=0;
	while(1)
	{
		nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept");

		char buffer[30];
		recv(nfd,buffer,30,0);
		int actors_count=atoi(buffer);
		recv(nfd,buffer,30,0);
		int set_count=atoi(buffer);
		printf("No of Actors%d\n",actors_count);
		printf("No of sets%d\n",set_count);	
		int i;
		for(i=0;i<actors_count;i++)
		{
			recv(nfd,buffer,30,0);
			printf("buf :%s\n",buffer);
			actors[i]=atoi(buffer);
		}
		for(i=0;i<set_count;i++)
		{
			recv(nfd,buffer,30,0);
			printf("buf2 :%s\n",buffer );

			sets[i]=atoi(buffer);
		}

		for(i=0;i<actors_count;i++)
		{
			printf("%d\n",actors[i]);
		}
		for(i=0;i<set_count;i++)
		{
			printf("%d\n",sets[i]);
		}

		int studfd=socket(AF_INET,SOCK_STREAM,0);
		error_func(studfd,"socket");
		serv_addr.sin_port=htons(5000);
		error_func(connect(studfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect");

		char setbuffer[30];
		sprintf(setbuffer,"%d",set_count);
		printf("new set%s\n",setbuffer);
		send(studfd,setbuffer,30,0);
		for(i=0;i<set_count;i++)
		{
			sprintf(setbuffer,"%d",sets[i]);
			send(studfd,setbuffer,30,0);
			recv(studfd,setbuffer,30,0);
			int port=atoi(setbuffer);
			printf("%d\n",port);

			//connect to sets for permissions

			int setfd=socket(AF_INET,SOCK_STREAM,0);
			error_func(setfd,"socket");
			serv_addr.sin_port=htons(port);
			error_func(connect(setfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect");

			recv(setfd,setbuffer,30,0);
			int perm=atoi(setbuffer);
			if(perm==1)
			{
				sets_accp[s_a++]=setfd;
				printf("%s\n","permission given for set");
			}
			else
			{
				printf("%s\n","set is not available");
			}
		}

		int actfd=socket(AF_INET,SOCK_STREAM,0);
		error_func(actfd,"socket");
		serv_addr.sin_port=htons(6000);
		error_func(connect(actfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect");

		char actbuffer[30];
		sprintf(actbuffer,"%d",actors_count);
		printf("new set%s\n",actbuffer);
		send(actfd,actbuffer,30,0);

		for(i=0;i<actors_count;i++)
		{
			sprintf(actbuffer,"%d",actors[i]);
			send(actfd,actbuffer,30,0);
			recv(actfd,actbuffer,30,0);
			int port=atoi(actbuffer);
			printf("%d\n",port);

			//connect to sets for permissions

			int setfd=socket(AF_INET,SOCK_STREAM,0);
			error_func(setfd,"socket");
			serv_addr.sin_port=htons(port);
			error_func(connect(setfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"accept");

			recv(setfd,actbuffer,30,0);
			int perm=atoi(actbuffer);
			if(perm==1)
			{
				actors_accp[a_a++]=setfd;
				printf("setfd%d\n",setfd );
				printf("%s\n","permission given for actor");
			}
			else
			{
				printf("%s\n","actor is not available");
			}
		}


		//passing fd's to producer
		int usfd,unfd;
		struct sockaddr_un serv_addr1;
		serv_addr1.sun_family=AF_UNIX;
		strcpy(serv_addr1.sun_path,"echo_socket");
		usfd=socket(AF_UNIX,SOCK_STREAM,0);
		error_func(usfd,"socket error");
		error_func(connect(usfd,(struct sockaddr*)&serv_addr1,sizeof(serv_addr1)),"connect error");
		char newbuffer[30];
		send(usfd,"From client to server",30,0);
		recv(usfd,newbuffer,sizeof(buffer),0);
		printf("%s\n",newbuffer);

		char namebuf[30];
		sprintf(namebuf,"%d",s_a);
		send(usfd,namebuf,30,0);
		sprintf(namebuf,"%d",a_a);
		send(usfd,namebuf,30,0);

		for(i=0;i<s_a;i++)
		{
			send_file_descriptor(usfd,sets_accp[i]);
		}

		for(i=0;i<a_a;i++)
		{
			send_file_descriptor(usfd,actors_accp[i]);
		}


	}
}