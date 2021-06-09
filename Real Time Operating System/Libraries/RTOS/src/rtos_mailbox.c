/*
 * rtos_mailbox.c
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#include "rtos_mailbox.h"


void RTOS_mailBoxInit(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t bufferCapacity, uint32_t messageSize)
{
	ASSERT(pMailBox != NULL);
	ASSERT(pBuffer != NULL);
	ASSERT(bufferCapacity > 0);
	ASSERT(messageSize > 0);

	/* Initialize the MailBox */
	pMailBox->pBuffer = (uint32_t)pBuffer;
	/* To ensure message size alignment */
	pMailBox->bufferCapacity = (bufferCapacity / messageSize) * messageSize;
	pMailBox->messageSize = messageSize;
	pMailBox->readIndex = 0;
	pMailBox->writeIndex = 0;
	//RTOS_semaphoreInit(& pMailBox->mailBoxSemaphore, 1);
	RTOS_semaphoreInit(& pMailBox->emptySpacesSemaphore, bufferCapacity);
	RTOS_semaphoreInit(& pMailBox->usedSpacesSemaphore, 0);
}
RTOS_returnStatus RTOS_mailBoxSend(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t waitTicks)
{
	ASSERT(pMailBox != NULL);
	ASSERT(pBuffer != NULL);

	RTOS_returnStatus returnStatus;
	/* If buffer is full, block the process */
	returnStatus = RTOS_semaphoreWait(& pMailBox->emptySpacesSemaphore, waitTicks);

	if(returnStatus == RTOS_SUCCESS)
	{
		/* The mailbox is a shared resource so we need to protect it*/
		//RTOS_semaphoreWait(& pMailBox->mailBoxSemaphore, waitTicks);
		/* Copy the data into the mailbox buffer */
		memcpy((void*)(pMailBox->pBuffer + pMailBox->writeIndex), pBuffer, pMailBox->messageSize);
		/* Increment the write index, If we reached the end of the buffer then reset in the index */
		pMailBox->writeIndex = (pMailBox->writeIndex + pMailBox->messageSize) % pMailBox->bufferCapacity;
		/* Release the mailbox semaphore */
		//RTOS_semaphoreSignal(& pMailBox->mailBoxSemaphore);
		/* Increment the used spaces in the mailbox */
		RTOS_semaphoreSignal(& pMailBox->usedSpacesSemaphore);
	}

	return returnStatus;
}

RTOS_returnStatus RTOS_mailBoxReceive(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t waitTicks)
{
	ASSERT(pMailBox != NULL);
	ASSERT(pBuffer != NULL);

	RTOS_returnStatus returnStatus;
	/* If the mailbox is empty, block the process */
	returnStatus = RTOS_semaphoreWait(& pMailBox->usedSpacesSemaphore, waitTicks);

	if(returnStatus == RTOS_SUCCESS)
	{
		/* The mailbox is a shared resource so we need to protect it*/
		//RTOS_semaphoreWait(& pMailBox->mailBoxSemaphore, waitTicks);
		/* Copy the data from the mailbox buffer */
		memcpy(pBuffer, (void*)(pMailBox->pBuffer + pMailBox->readIndex), pMailBox->messageSize);
		/* Increment the read index, If we reached the end of the buffer then reset the index */
		pMailBox->readIndex = (pMailBox->readIndex + pMailBox->messageSize) % pMailBox->bufferCapacity;
		/* Release the mailbox semaphore */
		//RTOS_semaphoreSignal(& pMailBox->mailBoxSemaphore);
		/* Increment the empty spaces in the mailbox */
		RTOS_semaphoreSignal(& pMailBox->emptySpacesSemaphore);
	}

	return returnStatus;
}
