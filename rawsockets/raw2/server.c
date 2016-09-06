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
int total = 0;

int main(int argc, char const *argv[]) 
{
    struct sockaddr_in s_addr, d_addr;
    struct sockaddr_in addr; 
    int rsfd, n, len, val=1;
    char buffer[BUFSIZE], buf[BUFSIZE];
    rsfd = socket(AF_INET, SOCK_RAW, 101);
    setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));
    struct iphdr * iph; unsigned int iphdrlen;
    while(1) 
    {
        len = sizeof(addr);
        recvfrom(rsfd, buffer, BUFSIZE, 0, (struct sockaddr *)&addr, &len);
        struct sockaddr_in source,dest;
        unsigned short iphdrlen;     
        struct iphdr *iph = (struct iphdr *)buffer;
        iphdrlen =iph->ihl*4;
         
        strcpy(buf, buffer+iphdrlen);
        printf("\tMessage\n %s\n", buf);
   
    }
    return 0;
}