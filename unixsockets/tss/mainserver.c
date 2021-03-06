#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

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

int main()
{
	int sfd,sfds;
	struct sockaddr_un serv_addr,client_addr;
	struct sockaddr_un serv_addrs; //address of server1
	int len=sizeof(client_addr);
	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"main_echo_socket");

	serv_addrs.sun_family=AF_UNIX;
	strcpy(serv_addrs.sun_path,"echo_socket");

	sfds=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfds,"socket error");
	error_func(connect(sfds,(struct sockaddr*)&serv_addrs,sizeof(serv_addrs)),"connect error");
	send(sfds,"From mainserver to server",30,0);

	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");
	unlink(serv_addr.sun_path);
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");

	while(1)
	{
		int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept error");
		char buffer[30];
		recv(nfd,buffer,sizeof(buffer),0);
		printf("%s\n",buffer);
		send_file_descriptor(sfds,nfd);
	}

	return 0;

}