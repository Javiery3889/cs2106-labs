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
    sem_t sem;
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

    // Initialise semaphore's value to 1, so that first child does not wait indefinitely
    // initialise counter variable to 0 as well
    sem_init(&(shm->sem), 1, 1);
    shm->counter = 0;

    for (i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
        
    // Child process
    else if (pid == 0) {
        // Lock semaphore
        sem_wait(&(shm->sem));
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
        // Unlock semaphore
        sem_post(&(shm->sem));
        exit(EXIT_SUCCESS);
    }

    // Parent process
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    // Print the final value of the counter
    printf("Final counter value: %d\n", shm->counter);

    sem_destroy(&(shm->sem));
    // Detach and destroy the shared memory region
    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
