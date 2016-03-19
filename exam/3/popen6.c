#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <limits.h>

int main()
{
	char readbuf[30];
	while(1)
	{
		fscanf(stdin,readbuf,sizeof(readbuf));
		printf("%s\n",readbuf);
	}
}