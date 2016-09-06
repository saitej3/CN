#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define max_clients 3

pthread_t threads[max_clients];
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

void *func(void * arg)
{
    int nfd = *((int *)arg);
    printf("%s\n","thread started");

    char namebuffer[30];
    while(1)
    {
    	send(nfd,"sending from server1",30,0);
    	recv(nfd,namebuffer,sizeof(namebuffer),0);
    	printf("%s%s\n","from client :",namebuffer);
    	sleep(2);
    }
}

int main()
{
	int sfd;
	struct sockaddr_un serv_addr,client_addr;
	int len=sizeof(client_addr);
	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"echo_socket");

	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");
	unlink(serv_addr.sun_path);
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");

	int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept error");
	char buffer[30];
	recv(nfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);

	int clients_no=0;
	while(1)
	{
		int fd=recv_file_descriptor(nfd);
		printf("the descriptor is %d\n",fd);

		printf("%s\n","client accepted");
		pthread_create(&threads[clients_no++],NULL,func,(void *)(&fd));
	}
	
	return 0;

}