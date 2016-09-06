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

void error_func(int val,char *message)
{
	if(val<0)
		perror(message);
}

int main()
{
	sleep(2);
	printf("%s\n","came here");
	char buffer[30];
	fscanf(stdin,"%s",buffer);
	printf("%s\n",buffer);
	fprintf(stdout, "%s\n","hai from server1");
}