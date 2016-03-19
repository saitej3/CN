#include <iostream>
#include <cstdio>
using namespace std;


int main()
{
	int num;
	int parity;
	scanf("%d",&num);
	int temp=num;
	int count=0;
	while(num)
	{
		if(num & 1)
			count++;
		num=num>>1;
		
	}
	if(count%2==1)
		parity=1;
	else
		parity=0;

	printf("The parity bit is %d\n",parity);
	printf("And the transmitted number is %d\n",temp<<1|parity);
}
