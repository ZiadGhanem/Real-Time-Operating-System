/*
 * rtos_thread.c
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"

/* Current top thread priority */
static uint32_t currentTopPriority = MAX_PRIORITY_LEVEL - 1;
/* Pointer to the current running thread */
static RTOS_thread_t* pCurrentlyRunningThread = NULL;
/* RTOS ready lists */
static RTOS_list_t RTOS_readyList[MAX_PRIORITY_LEVEL];

/*
 * This function initializes the ready lists
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_threadReadyListsInit(void)
{
	uint32_t i;
	for(i = 0; i < MAX_PRIORITY_LEVEL; i++)
		RTOS_listInit(&RTOS_readyList[i]);
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
void RTOS_threadCreate(RTOS_thread_t* pThread, RTOS_stack_t* pStack, void* pFunction, uint32_t priority)
{
	ASSERT(pThread != NULL);
	ASSERT(pStack != NULL);
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
	 * R0
	 * R1
	 * R2
	 * R3
	 * R12
	 * LR
	 * PC
	 * xPSR
	 */

	/* Initialize the stack pointer */
	pThread->pStack = (uint32_t)pStack->stack + MAX_STACK_SIZE * 8 - 18 * 4;

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

	/* Set the thread's list item thread pointer */
	pThread->listItem.pThread = pThread;

	/* Add the thread to the ready list */
	RTOS_listAppend(&RTOS_readyList[priority], &pThread->listItem);

	/* Check if the new thread has a new highest priority */
	if(priority < currentTopPriority)
	{
		currentTopPriority = priority;
	}
	else
	{

	}

	/* Check if the scheduler has started and the currently running thread has a lower priority */
	if((pCurrentlyRunningThread != NULL) && (priority < pCurrentlyRunningThread->priority))
	{
		/* Invoke a pendSV exception */
	    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{

	}

}


/*
 * This function get the currently running thread
 * Inputs:
 *  None
 * Return:
 * 	Pointer to the running thread
 */
RTOS_thread_t* RTOS_threadGetRunning(void)
{
	return pCurrentlyRunningThread;
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

	pCurrentlyRunningThread = RTOS_readyList[currentTopPriority].pIndex->pThread;

}
