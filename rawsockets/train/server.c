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


void print_error(int val, char*msg){
	if(val < 0){
		perror(msg);
		exit(val);
	}
}


int rsfd;


int main()
{
	int i;
	int to_send = 0;

	for(i=0;i<3;i++)
	{
		printf("%s\n", "Enter 1 to send. 0 to stop");
		scanf("%d", &to_send);
		if(to_send == 0)
			break;
		int rsfd = socket(AF_INET, SOCK_RAW, 100+i);
		print_error(rsfd, "Failed to create raw socket");
		struct sockaddr_in client_addr;
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(8000+i);
		client_addr.sin_addr.s_addr = INADDR_ANY;

		int b = bind(rsfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
		print_error(b, "Failed to bind.");

		char *msg = (char*)malloc(sizeof(char)*100);
		strcpy(msg, "Hello world.");
		int s = sendto(rsfd, msg, sizeof(msg), 0 , (struct sockaddr *)&client_addr, sizeof(client_addr));
		print_error(s, "Failed to send message");
	}

	
}