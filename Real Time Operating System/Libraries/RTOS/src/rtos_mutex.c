/*
 * rtos_mutex.c
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */


#include "rtos.h"

/*
 * This function initializes the mutex
 * Inputs:
 *  pMutex -> Pointer to the mutex
 *  value -> Value to which the mutex will be initialized
 * Return:
 * 	None
 */
void RTOS_mutexInit(RTOS_mutex_t* pMutex, int32_t value)
{
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
 * Return:
 * 	None
 */

void RTOS_mutexLock(RTOS_mutex_t* pMutex)
{
	uint8_t success = 0;
	while(1)
	{
		/* Read the mutex value */
		/* If the mutex is free */
		if((int32_t)__LDREXW((uint32_t*) &pMutex->value) == RTOS_MUTEX_FREE)
		{
			/* Set the mutex as used, If we failed to set the mutex value then just keep trying */
			if(__STREXW(RTOS_MUTEX_USED, (uint32_t*) &pMutex->value) == 0)
			{
				success = 1;
				/* Data Memory Barrier */
				__DMB();
				break;
			}
			else
			{

			}
		}
		/* If the mutex is used then we failed to acquire the mutex */
		else
		{
			break;
		}
	}

	/* In case of failure to acquire the mutex */
	if(success != 1)
	{
		/* Block this process */
		RTOS_listItem_t* pRunningItem = &(RTOS_threadGetRunning()->listItem);

		/* Set the items' ordering value */
		pRunningItem->orderValue = ((RTOS_thread_t*)pRunningItem->pThread)->priority;

		/* Remove the thread from the ready list */
		RTOS_listRemove(pRunningItem);

		/* Add the thread to the semaphores' list */
		RTOS_listInsert(&pMutex->mutexList, pRunningItem);

		/* Invoke a pendSV exception */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{

	}
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
	if(pMutex->mutexList.numListItems > 0)
	{
		/* Remove a process from the list */
		RTOS_listItem_t* pRemovedItem = pMutex->mutexList.endItem.pPrev;
		RTOS_listRemove(pRemovedItem);
		/* Place this thread in the ready list */
		RTOS_threadAddToReadyList(pRemovedItem->pThread);
	}
	else
	{

	}
}
