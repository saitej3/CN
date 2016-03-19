#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


int main()
{
	  int sfd[3];
	  int max;
	  int nfd;
	  fd_set readset,tempset;
	  FD_ZERO(&readset);
	  socklen_t len;
	  //addresses
	  struct sockaddr_in serv_addr[3],client_addr;

	  int i;
	  for(i=0;i<3;i++)
	  {
	  	serv_addr[i].sin_family=AF_INET;
	    serv_addr[i].sin_port=htons(8000+i);
	    serv_addr[i].sin_addr.s_addr=INADDR_ANY;
	  }

	  for(i=0;i<3;i++)
	  {
	  	sfd[i]=socket(AF_INET,SOCK_STREAM,0);
	  	FD_SET(sfd[i],&readset);
	  	int val = 1;
        int result=setsockopt(sfd[i],SOL_SOCKET,SO_REUSEADDR, &val,sizeof(val));  if(result<0) printf("%s\n","sock\n");
	  	bind(sfd[i],(struct sockaddr*)&serv_addr[i],sizeof(serv_addr[i]));
	  	listen(sfd[i],3);
	  }

	  while(1)
	  {
	  	tempset=readset;
	  	select(FD_SETSIZE,&tempset,NULL,NULL,NULL);
	  	int x;
  		for(i=0;i<3;i++)
  		{
  			if(FD_ISSET(sfd[i],&tempset))
  			{
  				nfd=accept(sfd[i],(struct sockaddr*)&client_addr,&len);
  				char servnamebuffer[30];
  				sprintf(servnamebuffer,"%s%d","server",i+1);
  				printf("%s%s\n","servnamebuffer ",servnamebuffer);
  				int c=fork();
  				if(c==0)
  				{
  					printf("%s\n","child process entered");
  					for(i=0;i<3;i++)
  						close(sfd[i]);
  					dup2(nfd,0);
  					dup2(nfd,1);
  					execve(servnamebuffer,NULL,NULL);
  				}
  				else
  				{
  					//wait(NULL);
  					close(nfd);
  					continue;
  				}
  			}
	  		
	  	}
	  }
	  

	  
}