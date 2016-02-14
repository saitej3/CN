#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

int main()
{
	int fd=open("pipe",O_RDONLY);
	char buf[25];
	struct pollfd fdp[1];
	fdp[0].fd=fd;
	fdp[0].events=POLLIN;

	while(1)
	{

	poll(fdp,1,1000);
		if(fdp[0].revents & POLLIN )
		{
			int charRead = read(fd,buf,sizeof(buf));
			write(1, buf, charRead);
			return 0;
		}
	}

	// read(fd,buf,sizeof(buf));
	// printf("%s\n",buf);
	return 0;
}
