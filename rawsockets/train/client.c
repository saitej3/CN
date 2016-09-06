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
	if(val < 0)
	{
		perror(msg);
		exit(val);
	}
}


int rsfd;


int main()
{
	rsfd = socket(AF_INET, SOCK_RAW, 100);
	error_func(rsfd, "Failed to create raw socket");

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int to_send = 0;

	char *msg = (char *)malloc(sizeof(char)*100);
	while(1){
		printf("%s\n", "Waiting to receive message");
		int s = recvfrom(rsfd, msg, 100, 0 , (struct sockaddr *)&client_addr, &client_len);
		error_func(s, "Failed to receive");
		printf("%s\n",msg);
		fflush(stdout);
		memset(msg, 0 , sizeof(msg));

	}

}