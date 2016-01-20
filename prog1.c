#include<stdio.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<fcntl.h>
#define size 2

key_t key=1852;
key_t key1=1412;
key_t key2=2096;

int semflg=IPC_CREAT|0777;
int nsems=2;

void sem_acq(int semid,int sem_numb)
{
	struct sembuf sb;
	sb.sem_num=sem_numb;
	sb.sem_op=1;
	sb.sem_flg=0;
	semop(semid,&sb,1);
}


void sem_rel(int semid,int sem_numb)
{
	struct sembuf sb;
	sb.sem_num=sem_numb;
	sb.sem_op=-1;
	sb.sem_flg=0;
	semop(semid,&sb,1);
}

int main()
{
	int shmid,shmid1,semid;
	semid=semget(key,nsems,semflg);
	int i;
	for(i=0;i<nsems;i++)
	{
		semctl(semid,i,SETVAL,0);
	}
	shmid=shmget(key1,size,semflg);
	shmid1=shmget(key2,size,semflg);

	int *x=shmat(shmid,NULL,semflg);
	int *y=shmat(shmid1,NULL,semflg);
	*x=0;
	*y=0;
	int c=fork();

	if(c==0)
	{
		while(*x<25)
		{
			sem_acq(semid,0);
			sem_rel(semid,1);
			*y=*x+1;
			printf("y in p2 =%d\n",*y);
		}
	}
	else
	{

		while(*y<25)
		{
			sem_rel(semid,0);
			*x=*y+1;
			printf("x in p1 =%d\n",*x);
			sem_acq(semid,1);
		}
	}

	shmctl(shmid,IPC_RMID,NULL);
	shmctl(shmid1,IPC_RMID,NULL);
	for(i=0;i<nsems;i++)
	{
		semctl(semid,i,IPC_RMID);
	}
	return 0;
}


