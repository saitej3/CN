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

#define BUFSIZE 1024

void error_func(int val,char *message)
{
    if(val<0)
    {
        perror(message);
        exit(0);
    }
}

int main(int argc, char const *argv[])
 {
    struct sockaddr_in addr; 
    int rsfd, n, len, val=1; 
    char buffer[BUFSIZE], buf[128];
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
    memset(buffer, 0 , BUFSIZE);

    iph = (struct iphdr *)buffer;
    iph->version = 4; 
    iph->ihl = 5; 
    iph->tos = 0;
    iph->frag_off = 0;
    iph->ttl = 8;
    iph->protocol = 101;
    iph->saddr = inet_addr("127.0.1.1");
    iph->daddr = inet_addr("127.0.0.1");
    strcpy(buffer+sizeof(struct iphdr), buf);
    iph->tot_len = htons(sizeof(struct iphdr)+strlen(buf));

    if(sendto(rsfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, sizeof addr) < 0)
     {
        perror("sendto() ");
    }
        
    
    return 0;
}