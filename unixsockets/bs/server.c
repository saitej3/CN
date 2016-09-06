#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
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

int cfd[max_clients];
int clients_no=0;
int bsfd;
int pid;

void handler_func()
{
	kill(pid,SIGUSR1); //signal backupserver
	printf("%s\n","The server is about to shutdown");
	int i;
	printf("count of no_cients%d\n",clients_no);
	
	for(i=0;i<max_clients;i++)
	{
		if(cfd[i]==-1) break;
		printf("fd %d\n",cfd[i]);
		send_file_descriptor(bsfd,cfd[i]);
	}

	exit(0);
}



int main()
{
	//get pid of backupserver
	 int shmid;
     key_t key;
     int *shm, *s;

     key = 5678;
     shmid = shmget(key, sizeof(int), 0666);
     shm = (int*) shmat(shmid, NULL, SHM_RND);
     s=shm;
     pid=*s;
	int i=0;
	for(i=0;i<max_clients;i++)
	{
		cfd[i]=-1;
	}
	signal(SIGUSR1,handler_func); //send sigusr1 to shutdown the server
	printf("Process id:%d\n",getpid());
	
	struct sockaddr_un serv_addrs;
	serv_addrs.sun_family=AF_UNIX;
	strcpy(serv_addrs.sun_path,"back_echo_socket");

	bsfd=socket(AF_UNIX,SOCK_STREAM,0);
	error_func(bsfd,"socket error");

	error_func(connect(bsfd,(struct sockaddr*)&serv_addrs,sizeof(serv_addrs)),"connect error");
	char buffer[30];
	send(bsfd,"From server to backupserver",30,0);

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

	while(1)
	{
		printf("%s\n","came1");
		int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		printf("%s\n","came2");
		error_func(nfd,"accept error");
		cfd[clients_no++]=nfd;
		printf("%s\n","client accepted");
		pthread_create(&threads[clients_no],NULL,func,(void *)(&nfd));
	}
}