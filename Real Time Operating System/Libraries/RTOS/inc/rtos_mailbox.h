/*
 * rtos_mailbox.h
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_MAILBOX_H_
#define RTOS_INC_RTOS_MAILBOX_H_

typedef struct{
	uint32_t pBuffer;
	uint32_t bufferCapacity;
	uint32_t messageSize;
	uint32_t readIndex;
	uint32_t writeIndex;
	RTOS_semaphore_t mailBoxSemaphore, emptySpacesSemaphore, usedSpacesSemaphore;
}RTOS_mailBox_t;

void RTOS_SVC_mailBoxInit(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t bufferCapacity, uint32_t messageSize);
void RTOS_SVC_mailBoxSend(RTOS_mailBox_t* pMailBox, void* pBuffer);
void RTOS_SVC_mailBoxReceive(RTOS_mailBox_t* pMailBox, void* pBuffer);

void RTOS_mailBoxInit(RTOS_mailBox_t* pMailBox, void* pBuffer, uint32_t bufferCapacity, uint32_t messageSize);
void RTOS_mailBoxSend(RTOS_mailBox_t* pMailBox, void* pBuffer);
void RTOS_mailBoxReceive(RTOS_mailBox_t* pMailBox, void* pBuffer);

#endif /* RTOS_INC_RTOS_MAILBOX_H_ */
