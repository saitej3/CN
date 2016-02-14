#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
	mkfifo("pipe",0666);
	int fd=open("pipe",O_WRONLY|O_NONBLOCK);
	char buf[25];
	while( 1 ){
		int charRead = read(0, buf, 100);
		write(fd,buf, charRead);
		write(1, buf, charRead);
	}
	return 0;
}