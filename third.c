#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	//FILE * f;
	//f=popen("third",1);
	FILE *fp;
	char buf[128];
	fp=popen("/home/cs13/c137261/cn_lab2/CN/second.out","w");
	//fgets(buf,128,fp);
	printf("%s\n","hello");
}
