/*
 * rtos_spinlock.h
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_SPINLOCK_H_
#define RTOS_INC_RTOS_SPINLOCK_H_

#include "rtos.h"

#define RTOS_SPINLOCK_USED (uint32_t)0
#define RTOS_SPINLOCK_FREE (uint32_t)1

/** RTOS Mutex
 * value -> The value of the mutex
 */
typedef struct{
	uint32_t value;
}RTOS_spinLock_t;

void RTOS_SVC_spinInit(RTOS_spinLock_t* pSpinLock, uint32_t value);
RTOS_returnStatus RTOS_SVC_spinLock(RTOS_spinLock_t* pSpinLock);
void RTOS_SVC_spinUnlock(RTOS_spinLock_t* pSpinLock);

void RTOS_spinInit(RTOS_spinLock_t* pSpinLock, uint32_t value);
RTOS_returnStatus RTOS_spinLock(RTOS_spinLock_t* pSpinLock);
void RTOS_spinUnlock(RTOS_spinLock_t* pSpinLock);

#endif /* RTOS_INC_RTOS_SPINLOCK_H_ */
