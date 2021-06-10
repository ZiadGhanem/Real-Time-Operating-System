/*
 * rtos_bsemaphore.c
 *
 *  Created on: Jun 10, 2021
 *      Author: ziadyasser
 */


#include "rtos_bsemaphore.h"

/*
 * This function initializes the binary semaphore
 * Inputs:
 *  pBSemaphore -> Pointer to the binary semaphore
 *  value -> Value to which the binary semaphore will be initialized
 * Return:
 * 	None
 */
void RTOS_bSemaphoreInit(RTOS_bSemaphore_t* pBSemaphore, uint32_t value)
{
	ASSERT(pBSemaphore != NULL);
	ASSERT((value == RTOS_BSEMAPHORE_USED) || (value == RTOS_BSEMAPHORE_FREE));
	/* Initialize the binary semaphore list */
	RTOS_listInit(&pBSemaphore->bSemaphoreList);
	/* Set the binary semaphore value */
	pBSemaphore->value = value;
}


/*
 * This function locks the binary semaphore
 * Inputs:
 *  pBSemaphore -> Pointer to the binary semaphore
 *  waitTicks -> Number of systicks to wait before retrying
 * Return:
 * 	States whether the binary semaphore was acquired successfully or not
 */

RTOS_returnStatus RTOS_bSemaphoreWait(RTOS_bSemaphore_t* pBSemaphore, uint32_t waitTicks)
{
	uint8_t terminate = 0;
	uint32_t value;
	RTOS_returnStatus returnStatus = RTOS_FAIL;

	/* Keep trying */
	while(!terminate)
	{
		/* Load the binary semaphore value */
		value = __LDREXW(&pBSemaphore->value);
		/* Check if the binary semaphore can be acquired */
		if(value == RTOS_BSEMAPHORE_FREE)
		{
			/* Attempt to store the binary semaphore */
			if( __STREXW(RTOS_BSEMAPHORE_USED, &pBSemaphore->value) == 0)
			{
				/* Data Memory Barrier */
				__DMB();
				/* Set the binary semaphore as acquired */
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


	/* If the binary semaphore was not acquired successfully */
	if(returnStatus == RTOS_FAIL && waitTicks > 0)
	{
		/* Get the currently running task */
		RTOS_task_t* pTask = RTOS_taskGetRunning();
		/* Remove the task from the ready list */
		RTOS_listRemove(& pTask->listItem);
		/* Set the items ordering value for the binary semaphore list */
		pTask->eventListItem.orderValue = pTask->priority;
		/* Add the task to the binary semaphore list */
		RTOS_listInsert(&pBSemaphore->bSemaphoreList, & pTask->eventListItem);

		if(waitTicks != RTOS_WAITFOREVER)
		{
			/* Add the task to timer's list */
			RTOS_taskAddToTimerList(pTask, waitTicks);
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
 * This function unlocks the binary semaphore
 * Inputs:
 *  pBSemaphore -> Pointer to the binary semaphore
 * Return:
 * 	None
 */
void RTOS_bSemaphoreSignal(RTOS_bSemaphore_t* pBSemaphore)
{
	/* Data Memory Barrier */
	__DMB();
	/* Unlock the binary semaphore */
	pBSemaphore->value = RTOS_BSEMAPHORE_FREE;
	/* Check if any tasks are blocked */
	if(pBSemaphore->bSemaphoreList.numListItems > 0)
	{
		/* Remove a task from the binary semaphore list */
		RTOS_task_t* pTask = pBSemaphore->bSemaphoreList.endItem.pPrev->pTask;
		RTOS_listRemove(& pTask->eventListItem);
		/* Check if the task was delayed then remove it from delay list */
		if(pTask->listItem.pList != NULL)
		{
			RTOS_listRemove(& pTask->listItem);
		}
		else
		{

		}
		/* Place this task in the ready list */
		RTOS_taskAddToReadyList(pTask);

	}
	else
	{

	}
}
