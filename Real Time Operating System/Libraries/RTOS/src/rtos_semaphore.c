/*
 * rtos_semaphore.c
 *
 *  Created on: May 19, 2021
 *      Author: ziadyasser
 */

#include "rtos_semaphore.h"

/*
 * This function initializes the semaphore
 * Inputs:
 *  pSemaphore -> Pointer to the semaphore
 *  value -> Value to which the semaphore will be initialized
 * Return:
 * 	None
 */
void RTOS_semaphoreInit(RTOS_semaphore_t* pSemaphore, uint32_t value)
{
	ASSERT(pSemaphore != 0);
	/* Initialize the semaphores list */
	RTOS_listInit(&pSemaphore->semaphoreList);
	/* Set the semaphores' value */
	pSemaphore->value = value;
}


/*
 * This function waits for the semaphore
 * Inputs:
 *  pSemaphore -> Pointer to the semaphore
 *  waitTicks -> Number of systicks to wait before retrying
 * Return:
 * 	States whether the semaphore was acquired successfully or not
 */

RTOS_returnStatus RTOS_semaphoreWait(RTOS_semaphore_t* pSemaphore, uint32_t waitTicks)
{
	uint8_t terminate = 0;
	uint32_t value;
	RTOS_returnStatus returnStatus = RTOS_FAIL;

	/* Keep trying*/
	while(!terminate)
	{
		/* Load the semaphore value */
		value = __LDREXW(&pSemaphore->value);
		/* Check if the semaphore can be acquired */
		if(value > 0)
		{
			/* Decrement the semaphore value and attempt to store the semaphore */
			if( __STREXW(value - 1, &pSemaphore->value) == 0)
			{
				/* Data Memory Barrier */
				__DMB();
				/* Set the semaphore as acquired */
				returnStatus = RTOS_SUCCESS;
				/* End loop */
				terminate = 1;
			}
			else
			{

			}
		}
		else
		{
			/* End loop */
			terminate = 1;
		}
	}


	/* If the semaphore was not acquired successfully */
	if(returnStatus == RTOS_FAIL && waitTicks > 0)
	{
		/* Get the currently running thread */
		RTOS_thread_t* pThread = RTOS_threadGetRunning();
		/* Remove the thread from the ready list */
		RTOS_listRemove(& pThread->listItem);
		/* Set the items ordering value for the semaphores list */
		pThread->eventListItem.orderValue = pThread->priority;
		/* Add the thread to the semaphores list */
		RTOS_listInsert(&pSemaphore->semaphoreList, & pThread->eventListItem);

		/* Add the thread to timer's list */
		if(waitTicks != RTOS_WAITFOREVER)
		{
			RTOS_threadAddToTimerList(pThread, waitTicks);

		}
		else
		{

		}
		returnStatus = RTOS_DELAY;
		/* Invoke a pendSV exception */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{

	}

	return returnStatus;
}


/*
 * This function signals the semaphore
 * Inputs:
 *  pSemaphore -> Pointer to the semaphore
 * Return:
 * 	None
 */
void RTOS_semaphoreSignal(RTOS_semaphore_t* pSemaphore)
{
	/* Keep trying to acquire the semaphore */
	uint32_t value;
	do
	{
		/* Load the semaphore value */
		value = __LDREXW(&pSemaphore->value);
		/* Increment the semaphore value */
		value++;
	}
	while( __STREXW(value, &pSemaphore->value) == 1);

	/* Data Memory Barrier */
	__DMB();

	/* Check if any threads are blocked */
	if(pSemaphore->semaphoreList.numListItems > 0)
	{
		/* Remove a thread from the semaphore list */
		RTOS_thread_t* pThread = pSemaphore->semaphoreList.endItem.pPrev->pThread;
		RTOS_listRemove(& pThread->eventListItem);
		/* Check if the thread was delayed then remove it from delay list */
		if(pThread->listItem.pList != NULL)
		{
			RTOS_listRemove(& pThread->listItem);
		}
		else
		{

		}
		/* Place this thread in the ready list */
		RTOS_threadAddToReadyList(pThread);
	}
	else
	{

	}
}
