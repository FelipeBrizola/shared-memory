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
sem_t *mutexProcess;


void createMutex() {

  	mutexProcess = sem_open(SEM_NAME, O_CREAT, 0644, 1);
  	if (mutexProcess == SEM_FAILED) {
    	perror("Erro ao criar semafóro");
      	sem_unlink(SEM_NAME);
      	exit(-1);
	}
}

int main() {
  	int shmid;
  	key_t key;
  	char *shm,*s;
  
  	// Key da memoria compartilhada
  	key = 1000;

	// Cria mutexs de tamanho 1
  	createMutex();

  	// Cria segmento de memoria compartilhada
    // passando passando identificador, tamanho da memoria, criacao com permissao de escrita
  	shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
  	if (shmid < 0) {
      perror("Erro ao criar segmento de memória compartilhada");
      exit(-1);
    }

  	// Anexa a memoria virtual
  	shm = shmat(shmid, NULL, 0);

	// server fica em loop infinito
  	while (1) {
		
		// fecha semaforo do processo. processo fica bloqueado
		sem_wait(mutexProcess);

		// Le da memoria compartilhada,
		// escreve no terminal.
		// repete ate encontrar NULL
		for (s = shm; *s != NULL; s++)
            putchar(*s);
        putchar('\n');
  }

  exit(0);
}