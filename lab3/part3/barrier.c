#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

typedef struct shared_memory {
    sem_t barrier_sem;
    sem_t mutex_sem;
    int counter;
} shared_memory;
    
int nproc, shmid;
shared_memory *shm;

void init_barrier(int numproc) {
    nproc = numproc;

    shmid = shmget(IPC_PRIVATE, sizeof(shared_memory), IPC_CREAT | 0600);
    if (shmid == -1) {
        printf("Cannot create shared memory!\n");
        exit(1);
    }

    shm = (shared_memory *) shmat(shmid, NULL, 0);
    if (shm == (void *) -1) {
        printf("Cannot attach shared memory!\n");
        exit(1);
    }

    shm->counter = 0;
    sem_init(&shm->barrier_sem, 1, 0);
    sem_init(&shm->mutex_sem, 1, 1);
}

void reach_barrier() {
    // lock mutex to increment count
    sem_wait(&shm->mutex_sem);
    (shm->counter)++;
    // unlock after increment
    sem_post(&shm->mutex_sem);

    // if there are n processes waiting, this means that the last
    // process will call barrier and we release each process
    if (shm->counter == nproc) {
        sem_post(&shm->barrier_sem);
    } else {
        sem_wait(&shm->barrier_sem);
        // we release the process one by one after last process calls barrier
        sem_post(&shm->barrier_sem);
    }
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(&shm->barrier_sem);
        sem_destroy(&shm->mutex_sem);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, 0);
    }
}
