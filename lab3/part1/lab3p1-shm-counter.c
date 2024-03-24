#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#define NUM_CHILDREN 5

struct shared_memory {
    // sem array is indicate which process' turn is it based on index
    sem_t sem[NUM_CHILDREN];
    // mutex is used to indicate if counter variable is available
    sem_t mutex;
    int counter;
};

int main() {
    int i, shmid;
    struct shared_memory *shm;
    pid_t pid;

    // Create shared memory region that can store the struct with read and write permissions
    shmid = shmget(IPC_PRIVATE, sizeof(struct shared_memory), IPC_CREAT | 0600);
    if (shmid == -1) {
        printf("Cannot create shared memory!\n");
        exit(1);
    }

    // Attach the shared memory region to this process
    shm = (struct shared_memory *) shmat(shmid, NULL, 0);
    if (shm == (void *) -1) {
        printf("Cannot attach shared memory!\n");
        exit(1);
    }

    // counter resource is available initially
    sem_init(&(shm->mutex), 1, 1);
    // set all child process to block when executed
    for (i = 0; i < NUM_CHILDREN; i++) {
        sem_init(&(shm->sem[i]), 1, 0);
    }
    // initialise counter variable to 0 as well
    shm->counter = 0;

    // only allow child 1 to execute critical section
    sem_post(&(shm->sem[0]));

    for (i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        // block any other child process
        sem_wait(&(shm->sem[i]));
        // lock mutex 
        sem_wait(&(shm->mutex));
        // Child process
        printf("Child %d starts\n", i + 1);
        // Simulate some work
        for (int j = 0; j < 5; j++) {
            // Increment counter value
            (shm->counter)++;
            // Print value of counter 
            printf("Child %d increment counter %d\n", i + 1, shm->counter);
            fflush(stdout);
            usleep(250000);
        }
        printf("Child %d finishes with counter %d\n", i + 1, shm->counter);
        // unlock mutex
        sem_post(&(shm->mutex));
        // next child can be unblocked
        sem_post(&(shm->sem[i+1]));
        exit(EXIT_SUCCESS);
    }

    // Parent process
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
        sem_destroy(&(shm->sem[i]));
    }

    // Print the final value of the counter
    printf("Final counter value: %d\n", shm->counter);

    // Detach and destroy the shared memory region
    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
