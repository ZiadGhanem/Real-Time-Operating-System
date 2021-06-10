/*
 * rtos_mutex.h
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_MUTEX_H_
#define RTOS_INC_RTOS_MUTEX_H_

#include <rtos_task.h>
#include "rtos.h"

#define RTOS_MUTEX_USED (uint32_t)0
#define RTOS_MUTEX_FREE (uint32_t)1

/** RTOS Mutex
 * mutexList -> The mutex blocked list
 * value -> The value of the mutex
 */
typedef struct{
	RTOS_list_t mutexList;
	uint32_t value;
}RTOS_mutex_t;

void RTOS_SVC_mutexInit(RTOS_mutex_t* pMutex, uint32_t value);
RTOS_returnStatus RTOS_SVC_mutexLock(RTOS_mutex_t* pMutex, uint32_t waitTicks);
void RTOS_SVC_mutexUnlock(RTOS_mutex_t* pMutex);

void RTOS_mutexInit(RTOS_mutex_t* pMutex, uint32_t value);
RTOS_returnStatus RTOS_mutexLock(RTOS_mutex_t* pMutex, uint32_t waitTicks);
void RTOS_mutexUnlock(RTOS_mutex_t* pMutex);


#endif /* RTOS_INC_RTOS_MUTEX_H_ */
