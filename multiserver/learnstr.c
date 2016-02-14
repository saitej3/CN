#include <string.h>
#include <stdio.h>

int main()
{
	char buf[30];
	scanf("%s",buf);
	char *token=NULL;
	while((token=strtok(buf," "))!=NULL)
	{
		printf("%s\n",token);
		token=NULL;
	}	
}