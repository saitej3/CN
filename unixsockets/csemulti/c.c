#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define max_clients 1

pthread_t threads[max_clients];
void error_func(int val,char * message)
{
	if(val<0)
	{
		printf("%s\n",message);
		exit(1);
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
    	send(nfd,"sending from server c",30,0);
    	recv(nfd,namebuffer,sizeof(namebuffer),0);
    	printf("%s%s\n","from client :",namebuffer);
    	sleep(2);
    }
}

int main()
{
	int sfd,sfd1;
	struct sockaddr_un serv_addr,client_addr;
	struct sockaddr_un serv_addrs;

	int len=sizeof(client_addr);

	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"echo_socket_c");

	serv_addrs.sun_family=AF_UNIX;
	strcpy(serv_addrs.sun_path,"echo_socket_cs");

	sfd1=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd1,"socket error");
	error_func(connect(sfd1,(struct sockaddr*)&serv_addrs,sizeof(serv_addrs)),"connect error");
	send(sfd1,"From server c to server s",30,0);
	
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");
	unlink(serv_addr.sun_path);

	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");
	
	char buffer[30];
	int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept error");
	recv(nfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);

	int clients_no=0;
	while(1)
	{
		int fd=recv_file_descriptor(nfd);
		printf("File descriptor%d\n",fd);
		if(clients_no>=max_clients)
		{
			printf("%s\n","max reached of server c");
			send_file_descriptor(sfd1,fd);
		}
		else
			pthread_create(&threads[clients_no++],NULL,func,(void *)(&fd));
	}


}