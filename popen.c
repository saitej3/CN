#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char * arg[])
{
	FILE * fp;
	char buf[128];
	int i;
	for(i=0;i<argc;i++)
	{		
		char path1[128]="/home/cs13/c137261/cn_lab2/CN/";
		strcat(path1,arg[i]);
		//printf("%s\n",path1);
		fp=popen(path1,"r");
		fgets(buf,128,fp);
		pclose(fp);
		if(i==argc-1)
			printf("%s\n",buf);
		char path2[128]="/home/cs13/c137261/cn_lab2/CN/";
		strcat(path2,arg[i+1]);
		//printf("%s\n",path2);
		fp=popen(path2,"w");
		printf(buf);
		pclose(fp);
	}
}
