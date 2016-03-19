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
#include <sys/ipc.h>
#include <sys/shm.h>


int main()
{
	//shared memory locations
	 int shmid;
     key_t key;
     int *shm, *s;
     key = 5679;
     shmid = shmget(key, sizeof(int), 0666);
     shm = (int*) shmat(shmid, NULL, SHM_RND);
     s=shm;
     printf("the pid is %d\n",*s);
     kill(*s,SIGUSR1);
}