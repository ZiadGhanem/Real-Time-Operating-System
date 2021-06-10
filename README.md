# Real-Time-Operating-System

-----
## Table of contents
* [Introduction](#introduction)
* [General Info](#general-info)
* [Future Improvements](#future-improvements)
* [Resources](#resources)

----
## Introduction
- Real-time operating system (RTOS) is an operating system intended to serve real time application that process data as it comes in, mostly without buffer delay.
- In an RTOS, Processing time requirement are calculated in tenths of seconds increments of time. It is time-bound system that can be defined as fixed time constraints. In this type of system, processing must be done inside the specified constraints. Otherwise, the system will fail.
- In this project, I successfully managed to implement a real-time preemptive operating system with alot of functionality for the ARM Cortex CM4F and specifically for the STM32F429 Discovery board, but it can be easily ported to other ARM Cortex M3/M4 Processors.

----

## General Info
Many features are implemented such as:

### Scheduler:
- The RTOS scheduler is responsible for which task should run next. It uses a Highest Priority Next algorithm, If there more than one task with the same priority it employs a Round-Robin Schedule for them.

##### Implemented methods:
```c
void RTOS_SVC_schedulerStart(void)
```
> This function starts the RTOS scheduler. After calling the RTOS kernel has control over which tasks are executed and when.
The idle task is created automatically when the RTOS scheduler is started. 

### Tasks:
- A real time application that uses an RTOS can be structured as a set of independent tasks. Each task executes within its own context with no coincidental dependency on other tasks within the system or the RTOS scheduler itself. Only one task within the application can be executing at any point in time and the real time RTOS scheduler is responsible for deciding which task this should be.
- The RTOS scheduler may therefore repeatedly start and stop each task (swap each task in and out) as the application executes.
- As a task has no knowledge of the RTOS scheduler activity it is the responsibility of the real time RTOS scheduler to ensure that the processor context (register values, stack contents, etc) when a task is swapped in is exactly that as when the same task was swapped out. To achieve this each task is provided with its own stack. When the task is swapped out the execution context is saved to the stack of that task so it can also be exactly restored when the same task is later swapped back in.

##### Implemented methods:
```c
void RTOS_SVC_taskCreate(RTOS_task_t* pTask, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority);
```
> Create a new task and add it to the list of tasks that are ready to run. The stack has to be statically allocated by the user.

```c
RTOS_SVC_taskDelay(uint32_t waitTicks);
```
> Delay a task for a given number of ticks. The actual time that the task remains blocked depends on the tick rate.

```c
void RTOS_SVC_taskTerminate(RTOS_task_t* pTask);
```
> Remove a task from the RTOS kernels management. The task being deleted will be removed from all ready, blocked and event lists. If NULL is provided then the current running task is deleted.

### Counting Semaphores / Binary Semaphores:
- A semaphore, in its most basic form, is a protected integer variable that can facilitate and restrict access to shared resources in a multi-processing environment.
- Counting semaphores represent multiple resources, while binary semaphores, as the name implies, represents two possible states (generally 0 or 1; locked or unlocked).
- A semaphore can only be accessed using the following operations: wait() and signal(). wait() is called when a task wants access to a resource. This would be equivalent to the arriving customer trying to get an open table. If there is an open table, or the semaphore is greater than zero, then he can take that resource and sit at the table. If there is no open table and the semaphore is zero, that process must wait until it becomes available. signal() is called when a process is done using a resource, or when the patron is finished with his meal.

##### Implemented methods:
```c
void RTOS_SVC_bSemaphoreInit(RTOS_bSemaphore_t* pBSemaphore, uint32_t value);
void RTOS_SVC_semaphoreInit(RTOS_semaphore_t* pSemaphore, uint32_t value);
```
> Creates a Binary/Counting Semaphore with the desired initial count value.

```c
RTOS_returnStatus RTOS_SVC_bSemaphoreWait(RTOS_bSemaphore_t* pBSemaphore, uint32_t waitTicks);
RTOS_returnStatus RTOS_SVC_semaphoreWait(RTOS_semaphore_t* pSemaphore, uint32_t waitTicks);
```
> Attempts to take the semaphore, If the semaphore is not available it waits for the waitTicks value to see if it becomes free. waitTicks = RTOS_WAITFOREVER can be used to block the task until signal() is used. If waitTicks = 0 the call will return immediately if it fails to acquire the semaphore.

```c
void RTOS_SVC_bSemaphoreSignal(RTOS_bSemaphore_t* pBSemaphore);
void RTOS_SVC_semaphoreSignal(RTOS_semaphore_t* pSemaphore);

```
> Used to release a semaphore.

### Spinlocks:
- A spinlock is a lock which causes a task trying to acquire it to simply wait in a loop ("spin") while repeatedly checking if the lock is available. Since the task remains active but is not performing a useful task, the use of such a lock is a kind of busy waiting.
- Once acquired, spinlocks will usually be held until they are explicitly released. Because they avoid overhead from operating system process rescheduling or context switching, spinlocks are efficient if tasks are likely to be blocked for only short periods.

```c
void RTOS_SVC_spinInit(RTOS_spinLock_t* pSpinLock, uint32_t value);
```
> Creates a Spinlock with the desired initial count value.

```c
RTOS_returnStatus RTOS_SVC_spinLock(RTOS_spinLock_t* pSpinLock);
```
> Attempts to take the spinlock, If the spinlock is not available the task busy waits until it becomes available.

```c
void RTOS_SVC_spinUnlock(RTOS_spinLock_t* pSpinLock);
```
> Releases the spinlock.

### Mutexes:
- A mutex is almost the same as a binary semaphore, the only difference is that a mutex can be released only by the task that had acquired it.

```c
void RTOS_SVC_mutexInit(RTOS_mutex_t* pMutex, uint32_t value);
RTOS_returnStatus RTOS_SVC_mutexLock(RTOS_mutex_t* pMutex, uint32_t waitTicks);
void RTOS_SVC_mutexUnlock(RTOS_mutex_t* pMutex);
```
> Same as the binary semaphore

### Mailboxes:
- Mailboxes are the primary form of intertask communications. They can be used to send messages between tasks. They are used as thread safe FIFO (First In First Out) buffers with new data being sent to the back of the mailbox.
- When a task attempts to read from an empty mailbox the task will be placed into the Blocked state (so it is not consuming any CPU time and other tasks can run) until either data becomes available in the mailbox, or the block time expires.
- When a task attempts to write to a full mailbox the task will be placed into the Blocked state (so it is not consuming any CPU time and other tasks can run) until either space becomes available in the mailbox, or the block time expires.
If more than one task block on the same queue then the task with the highest priority will be the task that is unblocked first. 

```c
void RTOS_SVC_mailBoxInit(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t bufferCapacity, uint32_t messageSize);
```
> Creates a new mailbox, The user must provide a RAM buffer which is used to items that are contained in the mailbox. 

```c
RTOS_returnStatus RTOS_SVC_mailBoxSend(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t waitTicks);
```
> Adds an item to the mailbox, waitTicks is used to provide the amount of time the task should block waiting for space to become available in the mailbox. If waitTicks = 0 the call will return immediately if the mailbox is full.

```c
RTOS_returnStatus RTOS_SVC_mailBoxReceive(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t waitTicks);
```
> Receive an item from the mailbox, waitTicks is used to provide the  amount of time the task should block waiting for an item to receive should the mailbox be empty at the time of the call. If waitTicks = 0 the call will return immediately if the mailbox is empty.

----

## Future Improvements

- Implement Priority Inheritance mechanism.
- Implement Dynamic Memory Allocation methods.
- Implement Memory Protection mechanisms.

----

## Resources

- Operating Systems, Internals and Design Principles, Ninth Edition (Global Edition), By William Stallings.
- [ARM Cortex-M4 Course, By Ahmed Eldeep.](https://www.youtube.com/playlist?list=PLPFp2zIiTnxXjxS-tC0u6m5zuSSJ1NLFw)
- [Quantul Leeps, LLC.](https://www.youtube.com/c/StateMachineCOM/featured)
