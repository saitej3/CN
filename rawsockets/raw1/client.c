#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/un.h>
#include <stropts.h>
#include <poll.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>

#define PROTOCOL 100
#define PORT 9999 
// dummy port. POrt isnt actually required.


void error_func(int val, char*msg){
	if(val < 0){
		perror(msg);
		exit(val);
	}
}


int rsfd;


void print_ip(struct sockaddr_in *s){
	
	char ipstr[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	printf("%s\n", ipstr);
	fflush(stdout);
	
}


void print_ip_header(char* msg){

	struct sockaddr_in source,dest;
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)msg;
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

     
    printf("\n");
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-Fragment Offset  : %d\n",(unsigned int)iph->frag_off); 
    printf("   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n",ntohs(iph->check));
    printf("   |-Source IP        :");
    print_ip(&source);
    printf("   |-Destination IP   :");
    print_ip(&dest);
   
    
}


int main(){
	rsfd = socket(AF_INET, SOCK_RAW, 100);
	error_func(rsfd, "Failed to create raw socket");

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int to_send = 0;

	while(1)
	{
		printf("%s\n", "Waiting to receive message");
		char msg[100];
		int s = recvfrom(rsfd, msg, 100, 0 , (struct sockaddr *)&client_addr, &client_len);
		error_func(s, "Failed to receive");
		int i;
		for(i=0;i<80;i++)
		{
			msg[i]=msg[20+i];
		}
		printf("%s\n",msg);
		print_ip_header(msg);
		fflush(stdout);
		memset(msg, 0 , sizeof(msg));

	}

}