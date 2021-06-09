/*
 * rtos_mutex.c
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */


#include "rtos_mutex.h"

/*
 * This function initializes the mutex
 * Inputs:
 *  pMutex -> Pointer to the mutex
 *  value -> Value to which the mutex will be initialized
 * Return:
 * 	None
 */
void RTOS_mutexInit(RTOS_mutex_t* pMutex, uint32_t value)
{
	ASSERT(pMutex != NULL);
	ASSERT((value == RTOS_MUTEX_USED) || (value == RTOS_MUTEX_FREE));
	/* Initialize the mutex list */
	RTOS_listInit(&pMutex->mutexList);
	/* Set the mutex value */
	pMutex->value = value;
}


/*
 * This function locks the mutex
 * Inputs:
 *  pMutex -> Pointer to the mutex
 *  waitTicks -> Number of systicks to wait before retrying
 * Return:
 * 	States whether the mutex was acquired successfully or not
 */

RTOS_returnStatus RTOS_mutexLock(RTOS_mutex_t* pMutex, uint32_t waitTicks)
{
	uint8_t terminate = 0;
	uint32_t value;
	RTOS_returnStatus returnStatus = RTOS_FAIL;

	/* Keep trying */
	while(!terminate)
	{
		/* Load the mutex value */
		value = __LDREXW(&pMutex->value);
		/* Check if the mutex can be acquired */
		if(value == RTOS_MUTEX_FREE)
		{
			/* Attempt to store the mutex */
			if( __STREXW(RTOS_MUTEX_USED, &pMutex->value) == 0)
			{
				/* Data Memory Barrier */
				__DMB();
				/* Set the mutex as acquired */
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


	/* If the mutex was not acquired successfully */
	if(returnStatus == RTOS_FAIL && waitTicks > 0)
	{
		/* Get the currently running thread */
		RTOS_thread_t* pThread = RTOS_threadGetRunning();
		/* Remove the thread from the ready list */
		RTOS_listRemove(& pThread->listItem);
		/* Set the items ordering value for the mutex list */
		pThread->eventListItem.orderValue = pThread->priority;
		/* Add the thread to the mutex list */
		RTOS_listInsert(&pMutex->mutexList, & pThread->eventListItem);

		if(waitTicks != RTOS_WAITFOREVER)
		{
			/* Add the thread to timer's list */
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
 * This function unlocks the mutex
 * Inputs:
 *  pMutex -> Pointer to the mutex
 * Return:
 * 	None
 */
void RTOS_mutexUnlock(RTOS_mutex_t* pMutex)
{
	/* Data Memory Barrier */
	__DMB();
	/* Unlock the mutex */
	pMutex->value = RTOS_MUTEX_FREE;
	/* Check if any threads are blocked */
	if(pMutex->mutexList.numListItems > 0)
	{
		/* Remove a thread from the mutex list */
		RTOS_thread_t* pThread = pMutex->mutexList.endItem.pPrev->pThread;
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
