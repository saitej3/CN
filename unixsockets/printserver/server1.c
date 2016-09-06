#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

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

void error_func(int val,char * message)
{
  if(val<0)
  {
    printf("%s\n",message);
  }
}

int main()
{
  int pid=0;
  pid=fork();

  if(pid==0)
  {
      sleep(5);
      printf("%s\n","child process");
      int sfd;
      struct sockaddr_un serv_addr;
      serv_addr.sun_family=AF_UNIX;
      strcpy(serv_addr.sun_path,"echo_socket");

      sfd=socket(AF_UNIX,SOCK_STREAM,0);
      error_func(sfd,"socket error");
      error_func(connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"connect error");
      int fd=recv_file_descriptor(sfd);
      printf("fd is child: %d\n",fd); 
      char buffer[30];
      int nbytes;
      while(1)
      {
        scanf("parent bytes?%d",&nbytes);
        read(fd,buffer,nbytes);
        printf("child :%s\n",buffer);
        sleep(3);
      }
      send(sfd,"From client to server",30,0);
      recv(sfd,buffer,sizeof(buffer),0);
      printf("%s\n",buffer);
      close(sfd);
  }
  else
  {
      printf("%s\n","parent process");
      const char *filename = "sample.txt";
      int fd = open(filename, O_RDWR);
          
      printf("fd parent:%d\n",fd);
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
      send_file_descriptor(nfd,fd);

      int nbytes;
      char buffer1[30];
       while(1)
      {
         scanf("parent bytes?%d",&nbytes);
         read(fd,buffer1,nbytes);
         printf("parent :%s\n",buffer1);
         sleep(3);
      }
      char buffer[30];
      recv(nfd,buffer,sizeof(buffer),0);
      printf("%s\n",buffer);
      send(nfd,"hello from server",30,0);
      wait(NULL);
      close(nfd);
      close(sfd);
  }
}