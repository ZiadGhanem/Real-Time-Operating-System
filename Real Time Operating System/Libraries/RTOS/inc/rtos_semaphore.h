/*
 * rtos_semaphore.h
 *
 *  Created on: May 19, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_SEMAPHORE_H_
#define RTOS_INC_RTOS_SEMAPHORE_H_

/** RTOS Semaphores
 * semaphoreList -> The semaphore's blocked list
 * value -> The value of the semaphore
 */
typedef struct{
	RTOS_list_t semaphoreList;
	int32_t value;
}RTOS_semaphore_t;

void RTOS_SVC_semaphoreInit(RTOS_semaphore_t* pSemaphore, int32_t value);
void RTOS_SVC_semaphoreWait(RTOS_semaphore_t* pSemaphore);
void RTOS_SVC_semaphoreSignal(RTOS_semaphore_t* pSemaphore);

void RTOS_semaphoreInit(RTOS_semaphore_t* pSemaphore, int32_t value);
void RTOS_semaphoreWait(RTOS_semaphore_t* pSemaphore);
void RTOS_semaphoreSignal(RTOS_semaphore_t* pSemaphore);

#endif /* RTOS_INC_RTOS_SEMAPHORE_H_ */
