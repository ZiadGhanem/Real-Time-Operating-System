/*
 * rtos_semaphore.c
 *
 *  Created on: May 19, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"

/*
 * This function initializes the semaphore
 * Inputs:
 *  pSemaphore -> Pointer to the semaphore
 *  value -> Value to which the semaphore will be initialized
 * Return:
 * 	None
 */
void RTOS_semaphoreInit(RTOS_semaphore_t* pSemaphore, int32_t value)
{
	/* Initialize the semaphores' FIFO */
	RTOS_FIFOInit(&pSemaphore->semaphoreFIFO);
	/* Set the semaphores' value */
	pSemaphore->value = value;
}


/*
 * This function waits for the semaphore
 * Inputs:
 *  pSemaphore -> Pointer to the semaphore
 * Return:
 * 	None
 */

void RTOS_semaphoreWait(RTOS_semaphore_t* pSemaphore)
{
	/* Load the semaphore value */
	int32_t value = (int32_t)__LDREXW((uint32_t*) &pSemaphore->value);
	/* Decrement the semaphore value */
	value --;
	// Acquire the semaphore
	if( __STREXW(value,(uint32_t*) &pSemaphore->value) == 0)
	{
		/* Check if the semaphore value is bigger than or equal zero */
		if(value >= 0)
		{
			return;
		}
		else
		{

		}

	}
	else
	{

	}

	RTOS_listItem_t* runningItem = &(RTOS_threadGetRunning()->listItem);

	/* Remove the thread from the ready list */
	RTOS_listRemove(runningItem);

	/* Add the thread to the semaphores' FIFO */
	RTOS_FIFOAppend(&pSemaphore->semaphoreFIFO, runningItem);

	/* Invoke a pendSV exception */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}


/*
 * This function initializes the semaphore
 * Inputs:
 *  pSemaphore -> Pointer to the semaphore
 * Return:
 * 	None
 */
void RTOS_semaphoreSignal(RTOS_semaphore_t* pSemaphore)
{
	/* Load the semaphore value */
	int32_t value = (int32_t)__LDREXW((uint32_t*) &pSemaphore->value);
	/* Increment the semaphore value */
	value ++;
	// Acquire the semaphore
	if( __STREXW(value,(uint32_t*) &pSemaphore->value) == 0)
	{
		/* Check if the semaphore value is less than or equal zero */
		if(value <= 0)
		{
			/* Remove a process from the FIFO */
			RTOS_listItem_t* pRemovedItem = RTOS_FIFORemove(&pSemaphore->semaphoreFIFO);
			RTOS_thread_t* pRemovedThread = pRemovedItem->pThread;

			/* Place this thread in the ready list */
			RTOS_listAppend(RTOS_threadGetList(pRemovedThread->priority), pRemovedItem);

			/* Check if the thread has a new highest priority */
			if(pRemovedThread->priority < RTOS_threadGetTopPriority())
			{
				/* Set the new top priority */
				RTOS_threadSetTopPriority(pRemovedThread->priority);
				/* Invoke a pendSV exception */
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			}
			else
			{

			}
		}
		else
		{

		}
	}
}
