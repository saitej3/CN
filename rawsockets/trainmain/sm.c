#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

#define max_services 5
int k;

void error_func(int val,char *message)
{
	if(val<0)
	{
		printf("%s\n",message);
		//exit(1);
	}
}

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

int newfd;
void broadcast()
{
	struct sockaddr_un serv_addru;
	serv_addru.sun_family=AF_UNIX;
	int i;
	for(i=0;i<2;i++)
	{
		char namebuffer[30];
		sprintf(namebuffer,"%s%d","plat_socket",i+1);
		strcpy(serv_addru.sun_path,namebuffer);
		int bsfd=socket(AF_UNIX,SOCK_STREAM,0);
	    error_func(bsfd,"socket error");
	    error_func(connect(bsfd,(struct sockaddr*)&serv_addru,sizeof(serv_addru)),"connect error");
	    char buffer[30];
		send(bsfd,"From sm : train is coming",30,0);
		if(i==k)
			newfd=bsfd;	
	}
	
}
int plat_available[3];
int plat_fd[3];
int main()
{
	int i,j;
	for(i=0;i<3;i++)
	{
		plat_available[i]=1; //initally all are available
	}
	int tsfd;
	struct sockaddr_in serv_addr,client_addr;
	socklen_t len=sizeof(client_addr);

	serv_addr.sin_port=htons(8000);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;

	tsfd=socket(AF_INET,SOCK_STREAM,0);
	error_func(tsfd,"socket call error");
	int set = setsockopt(tsfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	error_func(set,"calling sockpot");
	error_func(bind(tsfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(tsfd,5),"listen error");

	while(1)
	{
		int nfd=accept(tsfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept error");
		printf("%s\n","accepted connection");

		//check for available platforms
		int flag=0;
		for(k=0;k<3;k++)
		{
			if(plat_available[k]==1)
			{
				plat_available[k]=0;	
				flag=1;
				break;
			}
		}

		if(flag==1)
		{
			printf("platform %d%s\n",k," is avialble sending fd..." );
			broadcast();
			send_file_descriptor(newfd,nfd);
		}
	}


}