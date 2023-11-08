#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

sem_t *mutexSem;

struct shared_memory{
    int buf [1];
};

//Simple error handler
void errorHandler(char error[]){
    shm_unlink("/sharedMem");
    printf("%s", error);
    exit(1);
}

//Takes numbers from table, adds them to a sum, and number in table to 0 ten times
void* consumer(void* arg){
    int* table = (int*) arg;
    int sum = 0;
    int i = 0;

    sleep(1);
    for(int c = 0; c < 10; ++c){
        while(table[0] == 0 && table[1] == 0){
           //Waits in loop if both slots are empty
        }

        //Critical Section
        sem_wait(mutexSem);
        sum += table[i];
        table[i] = 0;
        sem_post(mutexSem);
        //Critical Section End

        ++i;

        if(i == 2) i = 0; //Ensures no out of bounds aceeses in array
    }

    printf("Total of random numbers is: %d", sum);    
    pthread_exit(0);
}

int main(){
    struct shared_memory *sharedMemPtr; //Creates struct of shared memory
    int fd_shm;

    //Opens semaphore creaded by producer
    if((mutexSem = sem_open("/sem-mutex", 0, 0, 0)) == SEM_FAILED)
        errorHandler("Failed to open semaphore");

    //Opens shared memory created by producer
    if((fd_shm = shm_open("/sharedMem", O_RDWR, 0)) == -1)
        errorHandler("Failed to open shared memory");

    //Maps shared memory
    if((sharedMemPtr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0 )) == MAP_FAILED)
        errorHandler("Failed to map memory in conusmer");
    
    pthread_t threadID;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&threadID, &attr, consumer, (void*)sharedMemPtr->buf); //Creates one thread for producer
    pthread_join(threadID, NULL); //Program runs till consumer thread is done

    shm_unlink("/sharedMem");
}