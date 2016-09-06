#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/un.h>
#include <stropts.h>
#include <poll.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>
void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s\n",message);
		exit(1);
	}
}

static int recv_file_descriptor (int socket) 
{
 int sent_fd;
 struct msghdr message;
 struct iovec iov[1];
 struct cmsghdr *control_message = NULL;
 char ctrl_buf[CMSG_SPACE(sizeof(int))];
 char data[1];
 int res;

 memset(&message, 0, sizeof(struct msghdr));
 memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* For the dummy data */
 iov[0].iov_base = data;
 iov[0].iov_len = sizeof(data);

 message.msg_name = NULL;
 message.msg_namelen = 0;
 message.msg_control = ctrl_buf;
 message.msg_controllen = CMSG_SPACE(sizeof(int));
 message.msg_iov = iov;
 message.msg_iovlen = 1;

 if((res = recvmsg(socket, &message, 0)) <= 0)
  return res;

 /* Iterate through header to find if there is a file descriptor */
 for(control_message = CMSG_FIRSTHDR(&message);
     control_message != NULL;
     control_message = CMSG_NXTHDR(&message,
                                   control_message))
 {
  if( (control_message->cmsg_level == SOL_SOCKET) &&
      (control_message->cmsg_type == SCM_RIGHTS) )
  {
   return *((int *) CMSG_DATA(control_message));
  }
 }

 return -1;
}

pthread_t thread;

void * func()
{
	int rsfd = socket(AF_INET, SOCK_RAW, 100);
	perror("socket:");
	error_func(rsfd, "Failed to create raw socket");
	struct sockaddr_in client_addr1;
	socklen_t client_len = sizeof(client_addr1);
	int to_send = 0;
	char *msg = (char *)malloc(sizeof(char)*100);
	while(1){
		printf("%s\n", "Waiting to receive message");
		int s = recvfrom(rsfd, msg, 100, 0 , (struct sockaddr *)&client_addr1, &client_len);
		error_func(s, "Failed to receive");
		printf("MSG->[%s]\n", msg);
		fflush(stdout);
		memset(msg, 0 , sizeof(msg));

	}
}
int main(int argc,char* argv[])
{
	struct sockaddr_un serv_addr,client_addr;
	int len=sizeof(client_addr);
	serv_addr.sun_family=AF_UNIX;

	char namebuf[30];
	sprintf(namebuf,"%s%s","plat_socket",argv[1]);
	strcpy(serv_addr.sun_path,namebuf);
	int sfd;
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");
	unlink(serv_addr.sun_path);
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");

	pthread_create(&thread,NULL,func,NULL);
	while(1)
	{
		int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept error");
		char buffer[30];
		recv(nfd,buffer,sizeof(buffer),0);
		printf("%s\n",buffer);
		int tfd=recv_file_descriptor(nfd);
		printf("The fd is%d\n",tfd);
		recv(tfd,buffer,sizeof(buffer),0);
		printf("%s\n",buffer);
		recv(tfd,buffer,sizeof(buffer),0);
		printf("%s\n",buffer);

		send(nfd,"train is leaving",30,0);

	}
}