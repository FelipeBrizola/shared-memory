
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27
char SEM_NAME[]= "semaphore";

int main(int argc, char** argv) {
    int shmid;
    key_t key;
    char *shm,*s;
    sem_t *mutex;
    sem_t *mutexAst;

    char *message = argv[1];

    //name the shared memory segment
    key = 1000;

    //create & initialize existing semaphore
    mutex = sem_open(SEM_NAME,0,0644,0);
    if(mutex == SEM_FAILED) {
        perror("Erro no semafóro");
        sem_close(mutex);
        exit(-1);
    }

    //create & initialize existing semaphore
    mutexAst = sem_open(SEM_NAME,0,0644,0);
    if(mutexAst == SEM_FAILED) {
        perror("Erro no semafóro");
        sem_close(mutexAst);
        exit(-1);
    }

    //create the shared memory segment with this key
    shmid = shmget(key, SHMSZ, 0666);
    if(shmid < 0) {
        perror("Erro ao criar segmento de memória compartilhada");
        exit(-1);
    }

    //attach this segment to virtual memory
    shm = shmat(shmid, NULL, 0);

    s = shm;
  
    for (int i = 0; i < strlen(message); i += 1)
        *s++ = message[i];
        
    sem_post(mutex);

    // sempre termina message com NULL;
    *s = NULL;

    exit(0);
}