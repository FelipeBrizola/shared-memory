#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>

#define SHMSZ     27

struct Memory {
    char *shm;
    sem_t semaphore;
};

main() {
    int shmid;
    key_t key;
    char  *s;

    struct Memory *memory;

    sem_init(&memory->semaphore, 0, 1);

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    

    /*
     * Now we attach the segment to our data space.
     */
    if ((memory = (struct Memory *) shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    sem_wait(&memory->semaphore);

    while (1) {
        if (sem_trywait(&memory->semaphore) < 0) {
            printf("tamo dentro");
            for (s = memory; *s != NULL; s++)
                putchar(*s);
            putchar('\n');
        }

    }  

    exit(0);
}