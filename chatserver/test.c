#include <stdio.h>
#include <stdlib.h>


int main()
{
	char buf[100];
	creat("sample.txt",0666);
	int fd=open("sample.txt",0666);
	int r=read(fd,buf,100);
	printf("%d\n",r);
	fprintf(stdout, "%s\n",buf);
}