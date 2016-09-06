#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sfd,nsfd,n,portno;
     socklen_t clientn;
     char *lower_args[] = {"./s1", NULL};
     char *upper_args[] = {"./s2", NULL};
     struct sockaddr_in myaddr,clientaddr;
     portno=atoi(argv[1]);
     sfd=socket(AF_INET,SOCK_STREAM,0);
     if(sfd<0)
        error("Socket not initilizated");
    bzero((char *)&myaddr,sizeof(myaddr));
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=INADDR_ANY;
    myaddr.sin_port=htons(portno);
    if((bind(sfd,(struct sockaddr *) &myaddr, sizeof(myaddr)))<0)
        error("Error on binding: ");
    listen(sfd,5);
    clientn=sizeof(clientaddr);
    char buffer[250];
    bzero(buffer,250);
    int pid=1;
    int service=0;
    while(1 && pid > 0)
    {
        nsfd=accept(sfd,(struct sockaddr *) &clientaddr, &clientn);
        n=read(nsfd,buffer,100);
        if(strcmp(buffer,"1\n")==0)
        {
            printf("Service 1 selected\n");
            service=1;
        }
        else if(strcmp(buffer,"2\n")==0)
        {
            printf("Service 2 selected\n");
            service=2;
        }
        else
        {
            printf("Invalid choice\n");
            exit(0);
        }
        int pid=fork();
        if(pid == 0){
            close(sfd);
            bzero(buffer,250);
            dup2(nsfd,0);
            dup2(nsfd,1);
            close(nsfd);
            if(service == 1)
            {
                printf("Exceing lower\n");
                execv("./lower",lower_args);
                printf("Failed\n");
            }
            else
            {
                printf("Exceing lower\n");
                execv("./upper",upper_args);
                 printf("Failed\n");
            }
        }
        else if(pid > 0){
            close(nsfd);
            continue;
        }
    }
}