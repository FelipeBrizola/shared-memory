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
sem_t *mutex;
sem_t *mutexAst;

// Tenta criar semaforo passando SEM_NAME.
// Como semaforo ja existe e o parametro O_EXCL esta ativo, busca semaforo.
void getMutex() {
    
    mutex = sem_open(SEM_NAME, O_EXCL, 0644, 0);
    if(mutex == SEM_FAILED) {
        perror("Erro no semafóro");
        sem_close(mutex);
        exit(-1);
    }

    mutexAst = sem_open(SEM_NAME, O_EXCL, 0644, 0);
    if(mutexAst == SEM_FAILED) {
        perror("Erro no semafóro");
        sem_close(mutexAst);
        exit(-1);
    }
}

int main(int argc, char** argv) {
    int shmid;
    key_t key;
    char *shm,*s;

    char *message = argv[1];

    //name the shared memory segment
    key = 1000;

    getMutex();

    // busca segmento de memoria compartilhada
    // passando passando identificador, tamanho da memoria e permissao de escrita
    shmid = shmget(key, SHMSZ, 0666);
    if (shmid < 0) {
        perror("Erro ao buscar segmento de memória compartilhada");
        exit(-1);
    }

    // Anexa a a memoria virtual
    shm = shmat(shmid, NULL, 0);
  
    // escreve na memoria compartilhada
    s = shm;
    for (int i = 0; i < strlen(message); i += 1)
        *s++ = message[i];
        
    // libera semaforo para server escrever message
    sem_post(mutex);

    // sempre termina message com NULL;
    *s = NULL;

    exit(0);
}