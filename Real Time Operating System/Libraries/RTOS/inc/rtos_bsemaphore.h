/*
 * rtos_bsemaphore.h
 *
 *  Created on: Jun 10, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_BSEMAPHORE_H_
#define RTOS_INC_RTOS_BSEMAPHORE_H_

#include <rtos_task.h>
#include "rtos.h"

#define RTOS_BSEMAPHORE_USED (uint32_t)0
#define RTOS_BSEMAPHORE_FREE (uint32_t)1

/** RTOS Binary Semaphore
 * mutexList -> The mutex blocked list
 * value -> The value of the mutex
 */
typedef struct{
	RTOS_list_t bSemaphoreList;
	uint32_t value;
}RTOS_bSemaphore_t;

void RTOS_SVC_bSemaphoreInit(RTOS_bSemaphore_t* pBSemaphore, uint32_t value);
RTOS_returnStatus bSemaphoreWait(RTOS_bSemaphore_t* pBSemaphore, uint32_t waitTicks);
void RTOS_SVC_bSemaphoreSignal(RTOS_bSemaphore_t* pBSemaphore);

void RTOS_bSemaphoreInit(RTOS_bSemaphore_t* pBSemaphore, uint32_t value);
RTOS_returnStatus RTOS_bSemaphoreWait(RTOS_bSemaphore_t* pBSemaphore, uint32_t waitTicks);
void RTOS_bSemaphoreSignal(RTOS_bSemaphore_t* pBSemaphore);

#endif /* RTOS_INC_RTOS_BSEMAPHORE_H_ */
