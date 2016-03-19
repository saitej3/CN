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


void handler_func(int sig)
{
     printf("got signal\n");
}

int main()
{
	int shmid;
    key_t key;
    int pid,available_no;
    int *shm,*s;
    pid = getpid();
	key = 5679;
	shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	shm = shmat(shmid, NULL, SHM_RND);
	s=shm;
	*s=pid;
	signal(SIGUSR1,handler_func);
	int a;

	pause();
}