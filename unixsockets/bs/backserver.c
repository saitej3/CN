#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define max_clients 5

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
    	send(nfd,"sending from backserver",30,0);
    	recv(nfd,namebuffer,sizeof(namebuffer),0);
    	printf("%s%s\n","from client :",namebuffer);
    	sleep(2);
    }
}
int sfd,nfd;
int csfd[max_clients];
int clients_no=0;

void thread_caller()
{
	int count=0;
	int i=0;
	for(i=0;i<clients_no;i++)
	{
		if(csfd[i]>0)
		{
			pthread_create(&threads[count++],NULL,func,(void *)(&csfd[i]));
		}
	}
}

void handler_func()
{
	printf("%s\n","The backup server is about to begin");
	int i;
	for(i=0;i<max_clients;i++)
	{
		int fd=recv_file_descriptor(nfd);
		printf("fd got is%d\n",fd);
		if(fd==-1) break;
		csfd[clients_no++]=fd;
	}

	thread_caller();
}


int main()
{
	//shared memory to store pid
	signal(SIGUSR1,handler_func);
	int shmid;
    key_t key;
    int pid;
    int *shm;

     pid = getpid();
     key = 5678;
     shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
     shm = shmat(shmid, NULL, SHM_RND);
     *shm=pid;	

     //backupserver begins
	struct sockaddr_un serv_addr,client_addr;
	int len=sizeof(client_addr);
	serv_addr.sun_family=AF_UNIX;
	strcpy(serv_addr.sun_path,"back_echo_socket");

	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(sfd,"socket error");
	unlink(serv_addr.sun_path);
	error_func(bind(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)),"bind error");
	error_func(listen(sfd,5),"listen error");

    nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
	error_func(nfd,"accept error");
	char buffer[30];
	recv(nfd,buffer,sizeof(buffer),0);
	printf("%s\n",buffer);
	
	while(1)
	{

	}
}
