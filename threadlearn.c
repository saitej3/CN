#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t thread1,thread2;
void* temp;
void* function1()
{

	printf("%s\n","this is from function1");

}

void* function2()
{
	printf("%s\n","this is from function 2" );
}
int main()
{
	pthread_create(&thread1,NULL,function1,NULL);
	pthread_create(&thread2,NULL,function2,NULL);
	pthread_join(thread1,&temp);
	pthread_join(thread2,&temp);

}