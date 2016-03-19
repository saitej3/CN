#include <iostream>
#include <cstdio>
using namespace std;


int main()
{
	int num;
	scanf("%d",&num);
	int count=0;
	while(num>>1)
	{
		if(num & 1)
			count++;
	}
	printf("%d\n",count);
}
