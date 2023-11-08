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

//Struct for anything in shared memory
struct shared_memory{
    int buf[1]; //Buffer for array
};

//Simple error handler that also unlinks memory to avoid issues if not properly done otherwise
void errorHandler(char error[]){
    shm_unlink("/sharedMem");
    printf("%s", error);
    exit(1);
}

sem_t *mutexSem; //Creates global semephore

//producer function
void* producer(void* arg){
    int* table = (int*) arg;
    int num;
    int i = 0;
    srand(time(NULL));

    //Adds random number to table between 1 - 10 ten times
    for(int c = 0; c < 10; ++c){
        num = rand() % 10 + 1;
        printf("Number: %d \n", num);

        while(table[0] != 0 && table[1] != 0){
            //Waits untill slot is open in table
        }

        //Critical Section
        if(sem_wait(mutexSem) == -1)
           assert(1 == 0);
        table[i] = num;
        sem_post(mutexSem);
        //Critical Section End

        ++i;
        if(i == 2) i = 0; //Ensures no out of bound access in array
    }

    pthread_exit(0);
}

int main(){
    struct shared_memory *sharedMemPtr; //Creates struct of shared memory
    int fd_shm;

    //Opening semaphore
    if((mutexSem = sem_open("/sem-mutex", O_CREAT, 0660, 0)) == SEM_FAILED)
        errorHandler("Failed to open semaphore");

    //Opening shared memory
    if((fd_shm = shm_open("/sharedMem", O_RDWR | O_CREAT | O_EXCL, 0660)) == -1) //Creates empty shared memory
        errorHandler("Failed to opem shared memory");
    
    //Sets size of shared memory to size of struct
    if(ftruncate(fd_shm, sizeof(struct shared_memory)) == -1)
        errorHandler("Failed to truncate memory");

    //Maps shared memory
    if((sharedMemPtr = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0 )) == MAP_FAILED)
        errorHandler("Failed to map memory in producer");

    //Opens semaphore before any producer thread creation in case
    sem_post(mutexSem);

    pthread_t threadID;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&threadID, &attr, producer, (void*)sharedMemPtr->buf); //Creates one thread for producer
    pthread_join(threadID, NULL); //Program runs till producer thread is done

    shm_unlink("/sharedMem");
}
