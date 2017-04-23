/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>


#define SHMSZ     27

struct Memory {
    char *shm;
    sem_t semaphore;
};

int main(int argc, char** argv) {
    int i;
    int shmid;
    key_t key;
    char *s;
    char *message = argv[1];

    struct Memory *memory;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
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

    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = memory;

    sem_wait(&memory->semaphore);

    for (i = 0; i < strlen(message); i += 1)
        *s++ = message[i];

    *s = NULL;

    sem_post(&memory->semaphore);

    exit(0);
}
