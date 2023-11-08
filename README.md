# Operating System HW 1

## How To Run

Compile and runs **only** on linux

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

-Creates semaphore and creates shared memory
-Starts producer code in a single thread
-Puts 10 random numbers onto a shared table of two in size

### Consumer

-Connects to the semaphore and shared memory
-Starts consumer code in a single thread
-"Cosnumes" 10 numbers in the table and adds them all up
