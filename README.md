# Operating System HW 1

Two programs that are meant to be run together that utilize the POSIX linux implementation of:

- Threads
- Semaphores
- Shared Memory

Code is written in C

## How To Run

Compile and runs ***only*** on linux

Compile code with commands:

```console
$gcc producer.c -pthread -lrt -o producer
$gcc consumer.c -pthread -lrt -o consumer
```

Run code with command:

```console
$./producer & ./consumer &
```

## What it does

Two programs used to showcase two processes using semahpores and shared memory

### Producer

- Creates semaphore and creates shared memory
- Starts producer code in a single thread
- Puts 10 random numbers onto a shared table of two in size
- Prints the numbers
- Synched up to have producer do this at the same time and on the same as the conusmer

### Consumer

- Connects to the semaphore and shared memory
- Starts consumer code in a single thread
- Consumes 10 numbers in the table and adds them all up and prints the result
- Prints the total of all the numbers added through the table
- Synched up to have consumer do this at the same time and on the same table as the producer
