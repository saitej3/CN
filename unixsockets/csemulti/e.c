#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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
    	send(nfd,"sending from server e",30,0);
    	recv(nfd,namebuffer,sizeof(namebuffer),0);
    	printf("%s%s\n","from client :",namebuffer);
    	sleep(2);
    }
}
int pid;
void gettspid()
{
	 int shmid;
     key_t key;
     int *shm, *s;
     key = 5679;
     shmid = shmget(key, sizeof(int), 0666);
     shm = (int*) shmat(shmid, NULL, SHM_RND);
     s=shm;
     pid=*s;
     printf("%d\n",pid);
}
int main()
{
	 
	int sfd,sfdse;
	struct sockaddr_un serv_addr,client_addr;
	struct sockaddr_un serv_addrse,client_addrse; //address of server1
	int lense=sizeof(client_addrse);
	int len=sizeof(client_addr);

	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"echo_socket_e");

	serv_addrse.sun_family=AF_UNIX;
	strcpy(serv_addrse.sun_path,"echo_socket_se");

	sfdse=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfdse,"socket error");
	
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");
	unlink(serv_addr.sun_path);
	unlink(serv_addrse.sun_path);

	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");
	error_func(bind(sfdse,(struct sockaddr*)&serv_addrse,sizeof(serv_addrse)),"bind error");
	error_func(listen(sfdse,5),"listen error");

	int nfdse=accept(sfdse,(struct sockaddr*)&client_addrse,&lense);
	error_func(nfdse,"accept error");
	char buffer[30];
	recv(nfdse,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);

	int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept error");
	recv(nfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	gettspid(); //get pid of ts
	struct pollfd fdp[2];

	fdp[0].fd=nfd;
	fdp[0].events=POLLRDNORM;

	fdp[1].fd=nfdse;
	fdp[1].events=POLLRDNORM;

	int clients_no=0;
	while(1)
	{
		error_func(poll(fdp,2,0),"poll error");

		if(fdp[0].events & fdp[0].revents)
		{
			printf("%s\n","0 main selected");
			int fd=recv_file_descriptor(nfd);
			printf("File descriptor%d\n",fd);
			if(clients_no>=max_clients)
			{
				kill(pid,SIGUSR1);
				exit(0);
			}	
			else
				pthread_create(&threads[clients_no++],NULL,func,(void *)(&fd));
		}

		else if(fdp[1].events & fdp[1].revents)
		{
			printf("%s\n","1 s server selected");
			int fd=recv_file_descriptor(nfdse);
			printf("File descriptor%d\n",fd);
			if(clients_no>=max_clients)
			{
				kill(pid,SIGUSR1);
				exit(0);
			}
			else
				pthread_create(&threads[clients_no++],NULL,func,(void *)(&fd));
		}
		else
			continue;
	}

}