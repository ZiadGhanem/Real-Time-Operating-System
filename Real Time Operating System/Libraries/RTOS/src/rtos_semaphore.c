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
	ASSERT(value >= 0);
	/* Initialize the semaphores list */
	RTOS_listInit(&pSemaphore->semaphoreList);
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
	/* Keep trying to acquire the semaphore */
	int32_t value;
	do
	{
		/* Load the semaphore value */
		value = (int32_t)__LDREXW((uint32_t*) &pSemaphore->value);
		/* Decrement the semaphore value */
		value--;
	}
	while( __STREXW(value,(uint32_t*) &pSemaphore->value) == 1);

	if(value < 0)
	{
		/* Block this process */
		RTOS_listItem_t* runningItem = &(RTOS_threadGetRunning()->listItem);

		/* Remove the thread from the ready list */
		RTOS_listRemove(runningItem);

		/* Add the thread to the semaphores' list */
		RTOS_listInsert(&pSemaphore->semaphoreList, runningItem);

		/* Invoke a pendSV exception */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{

	}
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
	int32_t value;
	do
	{
		/* Load the semaphore value */
		value = (int32_t)__LDREXW((uint32_t*) &pSemaphore->value);
		/* Increment the semaphore value */
		value++;
	}
	while( __STREXW(value,(uint32_t*) &pSemaphore->value) == 1);

	if(value <= 0)
	{
		/* Remove a process from the list */
		RTOS_listItem_t* pRemovedItem = pSemaphore->semaphoreList.endItem.pPrev;
		RTOS_listRemove(pRemovedItem);
		/* Place this thread in the ready list */
		RTOS_threadAddToReadyList(pRemovedItem->pThread);
	}
	else
	{

	}
}
