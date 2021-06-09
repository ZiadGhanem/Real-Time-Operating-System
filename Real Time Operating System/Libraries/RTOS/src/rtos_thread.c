/*
 * rtos_thread.c
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#include "rtos_thread.h"

/* Current top thread priority */
static uint32_t currentTopPriority = MAX_PRIORITY_LEVEL - 1;
/* Pointer to the current running thread */
static RTOS_thread_t* pRunningThread = NULL;
/* The ID of the current running thread */
static int32_t runningThreadID = -1;
/* RTOS ready lists */
static RTOS_list_t RTOS_readyList[MAX_PRIORITY_LEVEL];
/* RTOS delayed list */
static RTOS_list_t RTOS_delayList;
/* Thread ID numbering */
static int32_t numThreads = 0;

/*
 * This function initializes the used lists
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_threadListsInit(void)
{
	/* Initialize the ready lists */
	uint32_t i;
	for(i = 0; i < MAX_PRIORITY_LEVEL; i++)
		RTOS_listInit(&RTOS_readyList[i]);

	/* Initialize the delayed list */
	RTOS_listInit(&RTOS_delayList);
}

/*
 * This function creates a new thread and its it to the desired list
 * Inputs:
 *  pThread -> The RTOS thread object
 *  pStack -> The Stack of the new thread
 * 	pFunction -> The RTOS thread function
 * 	priority -> The thread priority level
 * Return:
 * 	None
 */
void RTOS_threadCreate(RTOS_thread_t* pThread, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority)
{
	ASSERT(pThread != NULL);
	ASSERT(pStack != NULL);
	ASSERT(stackSize <= MAX_STACK_SIZE);
	ASSERT(pFunction != NULL);
	ASSERT((priority < MAX_PRIORITY_LEVEL) && (priority >= 0));

	/* Stack: xPSR, PC, LR, R12, R3, R1, R1, R0 are automatically pushed,
	 * 		 while the rest are pushed manually during thread switching
	 * EXC_RETURN
	 * CONTROL
	 * R4
	 * R5
	 * R6
	 * R7
	 * R8
	 * R9
	 * R10
	 * R11
	 * S16 - S31, If FPU is on
	 * R0
	 * R1
	 * R2
	 * R3
	 * R12
	 * LR
	 * PC
	 * xPSR
	 */

	/* To ensure that the stack is 8 byte aligned */
	stackSize = (stackSize / 8) * 8;

	/* Initialize the stack pointer */
	pThread->pStack = (uint32_t)pStack + stackSize * 8 - 18 * 4;

	/* For testing */
	MEM32WORD(pThread->pStack + (2 << 2)) = 0x4;
	MEM32WORD(pThread->pStack + (3 << 2)) = 0x5;
	MEM32WORD(pThread->pStack + (4 << 2)) = 0x6;
	MEM32WORD(pThread->pStack + (5 << 2)) = 0x7;
	MEM32WORD(pThread->pStack + (6 << 2)) = 0x8;
	MEM32WORD(pThread->pStack + (7 << 2)) = 0x9;
	MEM32WORD(pThread->pStack + (8 << 2)) = 0x10;
	MEM32WORD(pThread->pStack + (9 << 2)) = 0x11;
	MEM32WORD(pThread->pStack + (10 << 2)) = 0x1;
	MEM32WORD(pThread->pStack + (11 << 2)) = 0x2;
	MEM32WORD(pThread->pStack + (12 << 2)) = 0x3;
	MEM32WORD(pThread->pStack + (13 << 2)) = 0x4;
	MEM32WORD(pThread->pStack + (14 << 2)) = 0x12;

	/* Initialize EXC_RETURN (Return to thread mode using PSP) */
	MEM32WORD(pThread->pStack) = 0xFFFFFFFDUL;

	/* Initialize the CONTROL Register (No FPU, PSP, Unprivileged mode)*/
	MEM32WORD(pThread->pStack + (1 << 2)) = 0x3;

	/* Initialize the program counter */
	MEM32WORD(pThread->pStack + (16 << 2)) = (uint32_t)pFunction;

	/* Initialize the xPSR register to only Thumb mode*/
	MEM32WORD(pThread->pStack + (17 << 2)) = 0x01000000;

	/* Set the priority level */
	pThread->priority = priority;

	/* Set thread ID */
	pThread->threadId = numThreads;
	numThreads++;

	/* Set the thread state to ready */
	pThread->threadState = RTOS_THREADREADY;

	/* Set the thread's list item thread pointer */
	pThread->listItem.pThread = pThread;
	pThread->eventListItem.pThread = pThread;

	/* Set the thread's list item to none */
	pThread->listItem.pList = NULL;
	pThread->eventListItem.pList = NULL;

	/* Add the thread to the ready list */
	RTOS_listInsertEnd(&RTOS_readyList[priority], &pThread->listItem);

	/* Check if the new thread has a new highest priority */
	if(priority < currentTopPriority)
	{
		currentTopPriority = priority;
	}
	else
	{

	}

	/* Check if the scheduler has started and the currently running thread has a lower priority */
	if((pRunningThread != NULL) && (priority < pRunningThread->priority))
	{
		/* Invoke a pendSV exception */
	    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{

	}

}


/*
 * This function returns the running thread
 * Inputs:
 *  None
 * Return:
 * 	Pointer to the running thread
 */
RTOS_thread_t* RTOS_threadGetRunning(void)
{
	return pRunningThread;
}

/*
 * This function adds the thread to its ready list
 * Inputs:
 *  pThread -> The thread to be inserted in the ready list
 * Return:
 * 	None
 */
void RTOS_threadAddToReadyList(RTOS_thread_t* pThread)
{
	RTOS_listItem_t* pListItem = &pThread->listItem;
	/* Add the thread to ready list */
	RTOS_listInsertEnd(&RTOS_readyList[pThread->priority], pListItem);
	/* Set the thread state as ready */
	pThread->threadState = RTOS_THREADREADY;
	/* Check if the thread has a new highest priority */
	if(pThread->priority < currentTopPriority)
	{
		/* Set the new top priority */
		currentTopPriority = pThread->priority;
	}
	else
	{

	}

	if(pRunningThread != NULL &&
	(pThread->priority < pRunningThread->priority))
	{
		/* Invoke a pendSV exception */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{
	}
}

/*
 * This function adds the thread to the timer's list
 * Inputs:
 *  pThread -> The thread to be inserted in the ready list
 *  waitTicks -> Number of systicks to be delayed
 * Return:
 * 	None
 */
void RTOS_threadAddToTimerList(RTOS_thread_t* pThread, uint32_t waitTicks)
{
	/* Get the list item */
	RTOS_listItem_t* pListItem = & pThread->listItem;

	/* Set the delay amount */
	pListItem->orderValue = RTOS_schedulerGetSystickCount() + waitTicks;

	/* Add the thread to the delayed list */
	RTOS_listInsert(&RTOS_delayList, pListItem);

	/* Set the thread state to blocked */
	pRunningThread->threadState = RTOS_THREADBLOCKED;
}


/*
 * This function switches the currently running thread
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_threadSwitch(void)
{
	/* Check if the current top priority list became empty */
	while(RTOS_readyList[currentTopPriority].numListItems == 0)
	{
		currentTopPriority++;
	}

	/* Get the next thread in the list */
	RTOS_readyList[currentTopPriority].pIndex = RTOS_readyList[currentTopPriority].pIndex->pNext;

	/* Check if the current index is the end item */
	if(RTOS_readyList[currentTopPriority].pIndex == (RTOS_listItem_t*) &RTOS_readyList[currentTopPriority].endItem)
	{
		/* Increment the index */
		RTOS_readyList[currentTopPriority].pIndex = RTOS_readyList[currentTopPriority].pIndex->pNext;
	}

	RTOS_thread_t* pThread = RTOS_readyList[currentTopPriority].pIndex->pThread;

	/* Set the currently running thread */
	pRunningThread = pThread;
	runningThreadID = pThread->threadId;

	/* Set the thread state to running */
	pThread->threadState = RTOS_THREADRUNNING;
}


/*
 * This function delays the currently running thread
 * Inputs:
 *  waitTicks -> Number of system ticks to be delayed
 * Return:
 * 	None
 */
void RTOS_threadDelay(uint32_t waitTicks)
{
	/* Remove the current thread from ready list */
	RTOS_listItem_t* pListItem = & pRunningThread->listItem;
	RTOS_listRemove(pListItem);

	/* Add the thread to timer list */
	RTOS_threadAddToTimerList(pListItem->pThread, waitTicks);

	/* Invoke a pendSV exception */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/*
 * This function checks if any of the blocked threads can be transferred to the ready list
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_threadDelayCheck(void)
{
	if(RTOS_delayList.numListItems > 0)
	{
		/* Check for delayed threads */
		RTOS_listItem_t* pListItem = RTOS_delayList.endItem.pPrev;
		if(pListItem->orderValue <= RTOS_schedulerGetSystickCount())
		{
			/* Remove the current thread from delay list */
			RTOS_listRemove(pListItem);

			/* Add the thread to the ready list */
			RTOS_threadAddToReadyList(pListItem->pThread);
		}
		else
		{

		}
	}
	else
	{
	}
}



/*
 * This function terminates a thread
 * Inputs:
 *  pThread -> Pointer to the thread to be deleted
 * Return:
 * 	None
 */
void RTOS_threadTerminate(RTOS_thread_t* pThread)
{

	if(pThread == NULL)
	{
		pThread = pRunningThread;
	}
	else
	{

	}

	/* Remove the threads items from any lists */
	if(pThread->listItem.pList != NULL)
	{
		RTOS_listRemove(& pThread->listItem);
	}
	else
	{

	}


	if(pThread->eventListItem.pList != NULL)
	{
		RTOS_listRemove(& pThread->eventListItem);
	}
	else
	{

	}

	/* Set the thread as terminated */
	pThread->threadState = RTOS_THREADTERMINATED;

	/* If the thread is the current running thread */
	if(pThread == pRunningThread)
	{
		/* Invoke a pendSV exception */
	    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}

}
