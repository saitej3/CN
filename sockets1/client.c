#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#define PORT 8888

/*
struct sockaddr_in
{
  short   sin_family; //must be AF_INET 
  u_short sin_port;
  struct  in_addr sin_addr;
  char    sin_zero[8]; // Not used, must be zero 
};
*/

int sfd; // socket fd




void print_error(int val, char*msg){
	if(val < 0){
		perror(msg);
		exit(val);
	}
}

int main(int argc, char *argv[]){


	int port; //port on which server runs
	int client_addr_len; // var to store len of the address of client. 
	char buffer[256], buffer1[256];

	struct sockaddr_in server_addr;

	// creating a socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	print_error(sfd, "error opening socket");




	// port = atoi(argv[1]);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	//Computer networks are big endian. This means that when little endian computers are going to pass integers over the network (IP addresses for example), they need to convert them to network byte order. Likewise, when the receive integer values over the network, they need to convert them back to their own native representation.
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// establishing the connection to server.
	int c = connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	print_error(c, "could not connect to server.");

	printf("%s\n", "connected to server");


	// reading from stdin and sending to server.
	int write_size;
	int read_size;
	while(1){
		// ssize_t in  = getline(&buffer, &write_size, stdin);
		gets(buffer);
		// print_error(in, "Failed to read from stdin");
		if(strcmp(buffer, "exit") == 0)
			exit(1);

		// printf("Sending:%s\n", buffer);
		int s = send(sfd, buffer, sizeof(buffer), 0);
		print_error(s, "Failed to send message to server");

		//receive a reply from the server
		int r = recv(sfd, buffer1, 256, 0);
		print_error(r, "Failed to receive a reply from the server");
		printf("Server:%s\n", buffer1);
		memset(buffer, 0, 256);
		memset(buffer1, 0, 256);
	}


	printf("Client disconnected\n");
	fflush(stdout);
	return 0;

}