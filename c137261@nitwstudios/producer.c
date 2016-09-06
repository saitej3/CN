
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
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
void error_func(int val,char * message)
{
	if(val<0)
	{
		perror(message);
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
int main()
{
	int sets[5];
	int actors[5];
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
		int nfd=accept(sfd,(struct sockaddr*)&client_addr,&len);
		error_func(nfd,"accept error");
		char buffer[30];
		recv(nfd,buffer,sizeof(buffer),0);
		printf("%s\n",buffer);
		send(nfd,"hello from server",30,0);

		recv(nfd,buffer,30,0);
		int set_count=atoi(buffer);
		recv(nfd,buffer,30,0);
		int actor_count=atoi(buffer);

		
		printf("Set count%d\n",set_count);
		printf("Actor count%d\n",actor_count);
		int i;
		for(i=0;i<set_count;i++)
		{
			sets[i]=recv_file_descriptor(nfd);
		}
		for(i=0;i<actor_count;i++)
		{
			actors[i]=recv_file_descriptor(nfd);
		}
		//sets[0]=6;
		for(i=0;i<set_count;i++)
		{
			printf("set fd:%d\n",sets[i]);
			write(sets[i],"payment done",30);
		}
		for(i=0;i<actor_count;i++)
		{
			printf("actor fd:%d\n",actors[i]);
			write(actors[i],"payment done",30);

		}


		struct sockaddr_in addr; 
	    int rsfd, n, val=1; 
	    char buffer1[1024], buf[128];
	    rsfd = socket(AF_INET, SOCK_RAW, 101);
	    error_func(rsfd,"socket");
	    setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));
	    memset((char *)&addr, 0, sizeof addr);
	    addr.sin_family = AF_INET;
	    addr.sin_port = 0;
	    addr.sin_addr.s_addr = INADDR_ANY;
	    struct iphdr * iph;
	    
	   memset(buf, 0, 128);
	    printf("Enter Message:");
	    scanf("%s", buf);
	    memset(buffer1, 0 , 1024);

	    iph = (struct iphdr *)buffer1;
	    iph->version = 4; 
	    iph->ihl = 5; 
	    iph->tos = 0;
	    iph->frag_off = 0;
	    iph->ttl = 8;
	    iph->protocol = 101;
	    iph->saddr = inet_addr("127.0.1.1");
	    iph->daddr = inet_addr("127.0.0.1");
	    strcpy(buffer1+sizeof(struct iphdr), buf);
	    iph->tot_len = htons(sizeof(struct iphdr)+strlen(buf));

	    if(sendto(rsfd, buffer1, sizeof(buffer1), 0, (struct sockaddr *)&addr, sizeof addr) < 0)
	     {
	        perror("sendto() ");
	    }
	}


	return 0;

}