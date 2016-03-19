#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#define PORT 8888


int sfd; // socket fd
int nsfd; //new fd when accept is called

void clean(int signo){
	printf("Shutting down server.\n");
	shutdown(nsfd, SHUT_RDWR);
	shutdown(sfd, SHUT_RDWR);
}

void print_error(int val, char*msg){
	if(val < 0){
		perror(msg);
		exit(val);
	}
}

int main(int argc, char *argv[]){
	if(signal(SIGINT, clean) == SIG_ERR){
  		printf("%s\n", "Error in catching SIGINT");
 	}

	int port; //port
	int client_addr_len; // len of address
	char buffer[256];

	struct sockaddr_in server_addr, client_addr;
	
	// creating a socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	print_error(sfd, "error opening socket");

	//setting resuseaddr so that we don't have to constantly wait to the socket to timeout out of it's TIMEWAIT state.
	int set = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    	print_error(set, "setsockopt(SO_REUSEADDR) failed");


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	int b = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	print_error(b, "Failed to bind.");

	listen(sfd, 3);

	printf("%s\n", "Server is listening for connections.");

	nsfd = accept(sfd,(struct sockaddr * )&client_addr, &client_addr_len );
	print_error(nsfd, "Failed in accepting connection");

	printf("Accepted connection from %d\n", inet_ntoa(client_addr.sin_addr));

	// reading messages from the client.
	int read_size;
	while( (read_size = recv(nsfd, buffer, 256, 0)) > 0){
		printf("Message:%s\n", buffer);
		int s = send(nsfd, buffer, sizeof(buffer), 0);
		print_error(s, "Failed to send message to client");
		memset(buffer, 0, 256);
	}


	printf("Client disconnected\n");
	fflush(stdout);
	return 0;

}