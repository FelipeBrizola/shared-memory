
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27
char SEM_NAME[]= "vik";

int main() {
  char ch;
  int shmid;
  key_t key;
  char *shm,*s;
  sem_t *mutex;

  	//name the shared memory segment
  	key = 1000;

  	//create & initialize semaphore
  	mutex = sem_open(SEM_NAME,O_CREAT,0644,1);
  	if(mutex == SEM_FAILED) {
    	perror("unable to create semaphore");
      	sem_unlink(SEM_NAME);
      	exit(-1);
	}

  	//create the shared memory segment with this key
  	shmid = shmget(key,SHMSZ,IPC_CREAT|0666);
  	if(shmid < 0) {
      perror("failure in shmget");
      exit(-1);
    }

  	//attach this segment to virtual memory
  	shm = shmat(shmid,NULL,0);

  	//start writing into memory
  	s = shm;

  	while (1) {
		char *message;

		while(*shm != '*') {
      		printf("VOU DORMIR%s\n", message);
      		sleep(1);
    	}

		for (s = shm; *s != NULL; s++)
            putchar(*s);
        putchar('\n');
		
		sem_wait(mutex);
		printf("minha mensagem");
		*shm = '*';
  }

  sem_close(mutex);
  sem_unlink(SEM_NAME);
  shmctl(shmid, IPC_RMID, 0);
  _exit(0);
}