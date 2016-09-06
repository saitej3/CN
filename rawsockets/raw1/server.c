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


void error_func(int val, char*msg){
	if(val < 0)
	{
		perror(msg);
		exit(val);
	}
}


int rsfd;



int main(){
	rsfd = socket(AF_INET, SOCK_RAW, 100);
	error_func(rsfd, "Failed to create raw socket");

	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(8000);
	client_addr.sin_addr.s_addr = INADDR_ANY;
	int b = bind(rsfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
	error_func(b, "Failed to bind.");
	int temp;
	while(1){
		printf("%s\n", "Enter");
		scanf("%d", &temp);
		char *msg = (char*)malloc(sizeof(char)*100);
		strcpy(msg, "Hello");
		int s = sendto(rsfd, msg, sizeof(msg), 0 , (struct sockaddr *)&client_addr, sizeof(client_addr));
		error_func(s, "Failed to send message");

	}

}